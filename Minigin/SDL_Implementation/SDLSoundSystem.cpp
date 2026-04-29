#include "SDLSoundSystem.h"
#include "Minigin/SoundSystem/InternalSSHelper.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <unordered_map>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class mg::SDLSoundSystem::Impl
{
public:
	Impl()
	{
		MIX_Init();
		m_pMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);

		m_thread = std::thread(&Impl::ProcessEvents, this);
	}

	~Impl()
	{
		m_isRunning = false;
		m_conditionVar.notify_all();

		if (m_thread.joinable())
		{
			m_thread.join();
		}

		Shutdown();
	}

	void Push(AudioEvent&& event)
	{
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_eventQueue.push(std::move(event));
		}

		m_conditionVar.notify_one();
	}

	void PreLoadSFX(std::string const& path)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_audioCache.find(path) != m_audioCache.end())
		{
			return;
		}

		MIX_Audio* audio = MIX_LoadAudio(m_pMixer, path.c_str(), false);
		if (!audio)
		{
			return;
		}

		m_audioCache[path] = audio;
	}

private:

	MIX_Mixer* m_pMixer{ nullptr };

	std::unordered_map<std::string, MIX_Audio*> m_audioCache;

	std::vector<MIX_Track*> m_activeTracks;
	MIX_Track* m_musicTrack{ nullptr };

	std::thread m_thread;
	std::mutex m_mutex;
	std::condition_variable m_conditionVar;
	std::queue<AudioEvent> m_eventQueue;

	std::atomic<bool> m_isRunning{ true };

	float m_musicVolume{ 1.0f };
	float m_SFXVolume{ 1.0f };
	bool m_isMuted{ false };

	MIX_Track* Play(MIX_Audio* audio, float volume, int loops = 0)
	{
		if (!audio)
		{
			return nullptr;
		}

		MIX_Track* track = MIX_CreateTrack(m_pMixer);
		if (!track)
		{
			return nullptr;
		}

		SDL_PropertiesID options = SDL_CreateProperties();
		if (!options)
		{
			MIX_DestroyTrack(track);
			return nullptr;
		}

		SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, static_cast<Sint64>(loops));

		MIX_SetTrackAudio(track, audio);
		MIX_SetTrackGain(track, volume);
		//MIX_SetTrackLoops(track, static_cast<int>(loops));
		// TODO: add setting individual track volume?
		MIX_PlayTrack(track, options);

		SDL_DestroyProperties(options);

		return track;
	}

	MIX_Audio* LoadTrack(std::string const& path)
	{
		auto it = m_audioCache.find(path);
		if (it != m_audioCache.end())
		{
			return it->second;
		}

		MIX_Audio* audio = MIX_LoadAudio(m_pMixer, path.c_str(), false);
		if (!audio)
		{
			return nullptr;
		}

		m_audioCache[path] = audio;
		return audio;
	}

	void UnloadTrack(MIX_Track* track)
	{
		if (!track || !m_pMixer)
		{
			return;
		}

		MIX_StopTrack(track, 0);
		MIX_DestroyTrack(track);
	}


	void ProcessEvents()
	{
		while (true)
		{
			AudioEvent event;
			{
				std::unique_lock<std::mutex> lock(m_mutex);
			
				m_conditionVar.wait(lock, [&]
					{
						return !m_eventQueue.empty() || !m_isRunning;
					});

				if (!m_isRunning && m_eventQueue.empty())
				{
					break;
				}

				event = std::move(m_eventQueue.front());
				m_eventQueue.pop();
			}

			HandleEvent(event);

			CleanupFinishedTracks();
		}
	}

	void HandleEvent(AudioEvent const& event)
	{
		if (!m_isRunning)
		{
			return;
		}

		switch (event.type)
		{
			case AudioEventType::PlaySFX:
			{
				auto* audio = LoadTrack(event.clip.filePath);
				if (!audio)
				{
					break;
				}

				auto* track = Play(audio, m_SFXVolume, event.clip.loops);

				if (track)
				{
					std::lock_guard<std::mutex> lock(m_mutex);
					m_activeTracks.push_back(track);
				}
				break;
			}

			case AudioEventType::PlayMusic:
			{
				auto* audio = LoadTrack(event.clip.filePath);
				if (!audio)
				{
					break;
				}

				if (m_musicTrack)
				{
					UnloadTrack(m_musicTrack);
					m_musicTrack = nullptr;
				}

				m_musicTrack = Play(audio, m_musicVolume, event.clip.loops);
				break;
			}

			case AudioEventType::StopMusic:
			{
				if (m_musicTrack)
				{
					UnloadTrack(m_musicTrack);
					m_musicTrack = nullptr;
				}
				break;
			}

			case AudioEventType::PauseMusic:
			{
				if (m_musicTrack)
				{
					MIX_PauseTrack(m_musicTrack);
				}
				break;
			}

			case AudioEventType::ResumeMusic:
			{
				if (m_musicTrack)
				{
					MIX_ResumeTrack(m_musicTrack);
				}
				break;
			}

			case AudioEventType::SetMusicVolume:
			{
				m_musicVolume = event.value;
				if (m_musicTrack)
				{
					MIX_SetTrackGain(m_musicTrack, m_musicVolume);
				}
				break;
			}

			case AudioEventType::SetSFXVolume:
			{
				m_SFXVolume = event.value;
				break;
			}

			case AudioEventType::ToggleMute:
			{
				m_isMuted = !m_isMuted;

				float gain = m_isMuted ? 0.f : 1.f;

				if (m_musicTrack)
				{
					MIX_SetTrackGain(m_musicTrack, gain * m_musicVolume);
				}

				
				std::vector<MIX_Track*> tracksCopy;
				{
					std::lock_guard<std::mutex> lock(m_mutex);
					tracksCopy = m_activeTracks;
				}

				for (auto* track : tracksCopy)
				{
					MIX_SetTrackGain(track, gain * m_SFXVolume);
				}
				break;
			}
		}
	}

	void CleanupFinishedTracks()
	{
		if (!m_isRunning)
		{
			return;
		}

		auto it = m_activeTracks.begin();

		while (it != m_activeTracks.end())
		{
			if (!MIX_TrackPlaying(*it))
			{
				UnloadTrack(*it);
				it = m_activeTracks.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void Shutdown()
	{
		if (m_pMixer)
		{
			MIX_StopAllTracks(m_pMixer, 0); 
		}

		while (!m_activeTracks.empty())
		{
			UnloadTrack(m_activeTracks.back());
			m_activeTracks.pop_back();
		}

		m_activeTracks.clear();

		if (m_musicTrack)
		{
			UnloadTrack(m_musicTrack);
			m_musicTrack = nullptr;
		}

		for (auto& [_, audio] : m_audioCache)
		{
			MIX_DestroyAudio(audio);
		}

		m_audioCache.clear();

		if (m_pMixer)
		{
			MIX_DestroyMixer(m_pMixer);
			m_pMixer = nullptr;
		}
		MIX_Quit();
	}
};


#pragma region pImpl_Wrapper
void mg::SDLSoundSystem::PreLoadSFX( AudioClip const& clip)
{
	m_pImpl->PreLoadSFX(clip.filePath);
}

void mg::SDLSoundSystem::PlaySFX( AudioClip const& clip)
{
	m_pImpl->Push({ AudioEventType::PlaySFX, clip });
}

void mg::SDLSoundSystem::PlayMusic(AudioClip const& clip)
{
	m_pImpl->Push({ AudioEventType::PlayMusic, clip });
}

void mg::SDLSoundSystem::StopMusic()
{
	m_pImpl->Push({ AudioEventType::StopMusic });
}

void mg::SDLSoundSystem::PauseMusic()
{
	m_pImpl->Push({ AudioEventType::PauseMusic });
}

void mg::SDLSoundSystem::ResumeMusic()
{
	m_pImpl->Push({ AudioEventType::ResumeMusic });
}

void mg::SDLSoundSystem::SetMusicVolume(float volume)
{
	m_pImpl->Push({ AudioEventType::SetMusicVolume, {}, volume });
}

void mg::SDLSoundSystem::SetSFXVolume(float volume)
{
	m_pImpl->Push({ AudioEventType::SetSFXVolume, {}, volume });
}

void mg::SDLSoundSystem::ToggleMute()
{
	m_pImpl->Push({ AudioEventType::ToggleMute });
}

mg::SDLSoundSystem::SDLSoundSystem()
	: m_pImpl(std::make_unique<Impl>())
{
}

mg::SDLSoundSystem::~SDLSoundSystem() = default;
#pragma endregion pImpl_Wrapper
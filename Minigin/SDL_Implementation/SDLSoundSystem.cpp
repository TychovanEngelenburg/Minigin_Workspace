#include "SDLSoundSystem.h"
#include "Minigin/SoundSystem/AudioEvent.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <unordered_map>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>

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

		m_isRunning.store(false);
		m_updateEventsCond.notify_all();

		if (m_thread.joinable())
		{
			m_thread.join();
		}

		if (m_pMusicTrack)
		{
			MIX_DestroyTrack(m_pMusicTrack);
			m_pMusicTrack = nullptr;
		}

		for (auto* track : m_pActiveTracks)
		{
			if (track)
			{
				MIX_DestroyTrack(track);
			}
		}
		m_pActiveTracks.clear();
	}

	void Push(AudioEvent&& event)
	{
		if (!m_isRunning.load())
		{
			return;
		}

		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_eventQueue.push(std::move(event));
		}
		m_updateEventsCond.notify_one();
	}



private:
	std::thread m_thread;
	std::mutex m_mutex;
	std::condition_variable m_updateEventsCond;

	std::queue<AudioEvent> m_eventQueue{};

	MIX_Mixer* m_pMixer{ nullptr };
	std::unordered_map<std::string, MIX_Audio*> m_audioCache{};
	std::vector<MIX_Track*> m_pActiveTracks{};
	MIX_Track* m_pMusicTrack{ nullptr };

	std::atomic<bool> m_isRunning{ true };
	std::atomic<float> m_musicVolume{ 1.0f };
	std::atomic<float> m_SFXVolume{ 1.0f };
	bool m_isMuted{ false };



	MIX_Track* CreateTrack(MIX_Audio* audio, float volume, int loops = 0)
	{

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

		MIX_PlayTrack(track, options);
		SDL_DestroyProperties(options);

		return track;
	}

	MIX_Audio* GetTrack(std::string const& path)
	{

		std::unique_lock<std::mutex> lock(m_mutex);
		if (auto it = m_audioCache.find(path); it != m_audioCache.end())
		{
			return it->second;
		}
		lock.unlock();

		MIX_Audio* newAudio = MIX_LoadAudio(m_pMixer, path.c_str(), false);
		if (!newAudio)
		{
			return nullptr;
		}

		lock.lock();
		auto& cached = m_audioCache[path];
		if (!cached)
		{

			cached = newAudio;
		}
		else
		{
			MIX_DestroyAudio(newAudio);
		}
		return cached;

	}

	void CleanTracks()
	{
		if (!m_isRunning.load())
		{
			return;
		}

		std::lock_guard<std::mutex> lock(m_mutex);
		m_pActiveTracks.erase(
			std::remove_if(m_pActiveTracks.begin(), m_pActiveTracks.end(), [](MIX_Track* track)
				{
					if (track == nullptr)
					{
						return true;
					}

					if (!MIX_TrackPlaying(track))
					{
						MIX_DestroyTrack(track);
						return true;
					}

					return false;
				}), m_pActiveTracks.end());

	}

	void PreLoadSFX(std::string const& path)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_audioCache.find(path) != m_audioCache.end())
		{
			return;
		}
		lock.unlock();

		MIX_Audio* audio = MIX_LoadAudio(m_pMixer, path.c_str(), false);
		if (!audio)
		{
			return;
		}

		lock.lock();
		m_audioCache[path] = audio;
	}

	void HandleEvent(AudioEvent const& event)
	{
		switch (event.type)
		{
			case AudioEventType::PreloadSFX:
			{
				PreLoadSFX(event.clip.filePath);
				break;
			}

			case AudioEventType::PlaySFX:
			{
				auto* audio = GetTrack(event.clip.filePath);
				if (!audio)
				{
					break;
				}

				auto* track = CreateTrack(audio, m_SFXVolume.load(), event.clip.loops);

				if (track)
				{
					std::lock_guard<std::mutex> lock(m_mutex);
					m_pActiveTracks.push_back(track);
				}
				break;
			}

			case AudioEventType::PlayMusic:
			{
				auto* audio = GetTrack(event.clip.filePath);
				if (!audio)
				{
					break;
				}

				if (m_pMusicTrack)
				{
					MIX_DestroyTrack(m_pMusicTrack);
					m_pMusicTrack = nullptr;
				}

				m_pMusicTrack = CreateTrack(audio, m_musicVolume.load(), event.clip.loops);
				break;
			}

			case AudioEventType::StopMusic:
			{
				if (m_pMusicTrack)
				{
					MIX_DestroyTrack(m_pMusicTrack);
					m_pMusicTrack = nullptr;
				}
				break;
			}

			case AudioEventType::PauseMusic:
			{
				if (m_pMusicTrack)
				{
					MIX_PauseTrack(m_pMusicTrack);
				}
				break;
			}

			case AudioEventType::ResumeMusic:
			{
				if (m_pMusicTrack)
				{
					MIX_ResumeTrack(m_pMusicTrack);
				}
				break;
			}

			case AudioEventType::SetMusicVolume:
			{
				m_musicVolume.store(event.value);
				if (m_pMusicTrack)
				{
					MIX_SetTrackGain(m_pMusicTrack, m_musicVolume.load());
				}
				break;
			}

			case AudioEventType::SetSFXVolume:
			{
				m_SFXVolume.store(event.value);
				break;
			}

			case AudioEventType::ToggleMute:
			{
				m_isMuted = !m_isMuted;

				float gain = m_isMuted ? 0.f : 1.f;

				if (m_pMusicTrack)
				{
					MIX_SetTrackGain(m_pMusicTrack, gain * m_musicVolume.load());
				}


				std::vector<MIX_Track*> tracksCopy;
				{
					std::lock_guard<std::mutex> lock(m_mutex);
					tracksCopy = m_pActiveTracks;
				}

				for (auto* track : tracksCopy)
				{
					MIX_SetTrackGain(track, gain * m_SFXVolume.load());
				}
				break;
			}
		}
	}

	void ProcessEvents()
	{
		while (true)
		{
			AudioEvent event;

			{
				std::unique_lock<std::mutex> lock(m_mutex);

				m_updateEventsCond.wait(lock, [&]
					{
						return !m_eventQueue.empty() || !m_isRunning.load();
					});

				if (m_eventQueue.empty() && !m_isRunning.load())
				{
					break;
				}

				event = std::move(m_eventQueue.front());
				m_eventQueue.pop();
			}

			HandleEvent(event);

			CleanTracks();
		}
	}
};


#pragma region pImpl_Wrapper
void mg::SDLSoundSystem::PreLoadSFX(AudioClip const& clip)
{
	m_pImpl->Push({ AudioEventType::PreloadSFX, clip });
}

void mg::SDLSoundSystem::PlaySFX(AudioClip const& clip)
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
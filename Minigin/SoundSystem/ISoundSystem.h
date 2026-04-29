#ifndef I_SOUNDSYSTEM_H
#define I_SOUNDSYSTEM_H
#include <string>

namespace mg
{
	struct AudioClip
	{
		std::string filePath;
		std::string name;
		int loops;
	};

	class ISoundSystem
	{
	public:
	public:
		virtual ~ISoundSystem() = default;

		virtual void PreLoadSFX(AudioClip const& clip) = 0;

		virtual void PlaySFX(AudioClip const& clip) = 0;
		virtual void PlayMusic(AudioClip const& clip) = 0;

		virtual void StopMusic() = 0;

		virtual void PauseMusic() = 0;
		virtual void ResumeMusic() = 0;

		virtual void SetMusicVolume(float volume) = 0;
		virtual void SetSFXVolume(float volume) = 0;

		virtual void ToggleMute() = 0;
	};
}

#endif // ! I_SOUNDSYSTEM_H

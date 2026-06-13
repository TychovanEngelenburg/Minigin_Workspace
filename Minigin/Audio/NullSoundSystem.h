#ifndef NULL_SOUNDSYSTEM_H
#define NULL_SOUNDSYSTEM_H

#include "Minigin/Audio/ISoundSystem.h"

namespace mg
{
	class NullSoundSystem final : public ISoundSystem
	{
        void PreLoadSFX(AudioClip const&) override {};

        void PlaySFX(AudioClip const&) override {}
        void PlayMusic(AudioClip const&) override {}

        void StopMusic() override {}

        void PauseMusic() override {}
        void ResumeMusic() override {}

        void SetMusicVolume( float volume) override {}
        void SetSFXVolume(float volume) override {}

        void ToggleMute() override { }
	};
}

#endif // ! NULL_SOUNDSYSTEM_H

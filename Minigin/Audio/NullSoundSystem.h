#ifndef NULL_SOUNDSYSTEM_H
#define NULL_SOUNDSYSTEM_H

#include "Minigin/Audio/ISoundSystem.h"

namespace mg
{
	class NullSoundSystem final : public ISoundSystem
	{
        void PreLoadSFX([[maybe_unused]] AudioClip const& clip) override {};

        void PlaySFX([[maybe_unused]] AudioClip const& clip) override {}
        void PlayMusic([[maybe_unused]] AudioClip const& clip) override {}

        void StopMusic() override {}

        void PauseMusic() override {}
        void ResumeMusic() override {}

        void SetMusicVolume([[maybe_unused]] float volume) override {}
        void SetSFXVolume([[maybe_unused]] float volume) override {}

        void ToggleMute() override { }
	};
}

#endif // ! NULL_SOUNDSYSTEM_H

#ifndef SDL_SOUNDSYSTEM_H
#define SDL_SOUNDSYSTEM_H
#include "Minigin/SoundSystem/ISoundSystem.h"
#include <memory>

namespace mg
{
    class SDLSoundSystem final : public ISoundSystem
    {
    public:
        void PreLoadSFX(AudioClip const& clip) override;

        void PlaySFX(AudioClip const& clip) override;
        void PlayMusic(AudioClip const& clip) override;

        void StopMusic() override;

        void PauseMusic() override;
        void ResumeMusic() override;

        void SetMusicVolume(float volume) override;
        void SetSFXVolume(float volume) override;

        bool ToggleMute() override;

        SDLSoundSystem();

        ~SDLSoundSystem() override;
        SDLSoundSystem(const SDLSoundSystem&) = delete;
        SDLSoundSystem& operator=(const SDLSoundSystem&) = delete;
        SDLSoundSystem(SDLSoundSystem&&) noexcept = delete;
        SDLSoundSystem& operator=(SDLSoundSystem&&) noexcept = delete;

    private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl;
    };
}

#endif // !SDL_SOUNDSYSTEM_H

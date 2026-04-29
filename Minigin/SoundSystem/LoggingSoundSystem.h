#ifndef LOGGING_SOUNDSYSTEM_H
#define LOGGING_SOUNDSYSTEM_H

#include "Minigin/SoundSystem/ISoundSystem.h"
#include <memory>
#include <iostream>
#include <concepts>

namespace mg
{
    template<typename SoundSystem>
    class LoggingSoundSystem final : public ISoundSystem
    {
    public:
        LoggingSoundSystem()
            : m_pSoundSystem(std::make_unique<SoundSystem>())
        {
            static_assert(std::derived_from<SoundSystem, ISoundSystem>,
                "SoundSystem must derive from ISoundSystem");
        }

        ~LoggingSoundSystem() override = default;

        LoggingSoundSystem(const LoggingSoundSystem&) = delete;
        LoggingSoundSystem& operator=(const LoggingSoundSystem&) = delete;
        LoggingSoundSystem(LoggingSoundSystem&&) noexcept = delete;
        LoggingSoundSystem& operator=(LoggingSoundSystem&&) noexcept = delete;

        void PreLoadSFX(AudioClip const& clip) override
        {
            std::cout << "Requested load of audioclip: '" << clip.name << "' path: '" << clip.filePath << "'.\n";
            m_pSoundSystem->PreLoadSFX(clip);
        }

        void PlaySFX(AudioClip const& clip) override
        {
            std::cout << "Requested SFX: '" << clip.name << "' path: '" << clip.filePath << "'.\n";
            m_pSoundSystem->PlaySFX(clip);
        }

        void PlayMusic(AudioClip const& clip) override
        {
            std::cout << "Requested music: '" << clip.name << "' path: '" << clip.filePath << "'.\n";
            m_pSoundSystem->PlayMusic(clip);
        }

        void StopMusic() override
        {
            std::cout << "Requested the game music to be stopped.\n";
            m_pSoundSystem->StopMusic();
        }

        void PauseMusic() override
        {
            std::cout << "Requested the game music to be paused.\n";
            m_pSoundSystem->PauseMusic();
        }

        void ResumeMusic() override
        {
            std::cout << "Requested the game music to be resumed.\n";
            m_pSoundSystem->ResumeMusic();
        }

        void SetMusicVolume(float volume) override
        {
            std::cout << "Requested music volume: " << volume << ".\n";
            m_pSoundSystem->SetMusicVolume(volume);
        }

        void SetSFXVolume(float volume) override
        {
            std::cout << "Requested SFX volume: " << volume << ".\n";
            m_pSoundSystem->SetSFXVolume(volume);
        }

        void ToggleMute() override
        {
            std::cout << "Requested to mute all audio.\n";
            m_pSoundSystem->ToggleMute();
        }

    private:
        std::unique_ptr<SoundSystem> m_pSoundSystem;
    };
}

#endif //!LOGGING_SOUNDSYSTEM_H
#ifndef AUDIO_EVENT_H
#define AUDIO_EVENT_H

#include "ISoundSystem.h"
namespace mg
{
	enum class AudioEventType
	{
		PlaySFX,
		PlayMusic,
		StopMusic,
		PauseMusic,
		ResumeMusic,
		SetMusicVolume,
		SetSFXVolume,
		ToggleMute
	};

	struct AudioEvent
	{
		AudioEventType type{};
		AudioClip clip{};
		float value{};
	};
};
#endif // !AUDIO_EVENT_H
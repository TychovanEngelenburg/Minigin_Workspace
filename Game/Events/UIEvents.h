#ifndef UI_EVENTS_H
#define UI_EVENTS_H
#include "Game/Core/GameModes.h"
struct LivesChangedEvent
{
	int PlayerId;
	int Lives;
};

struct ScoreChangedEvent
{
	int PlayerId;
	int Score;
};

struct GameModeChangedEvent
{
	GameMode Mode; 
};

#endif // !UI_EVENTS_H
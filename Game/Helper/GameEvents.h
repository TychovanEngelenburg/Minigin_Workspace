#ifndef GAME_EVENTS_H
#define GAME_EVENTS_H

/// Simple holder for event data structs.
// In case this project gets big/needs many events these are to be split up over multiple files.

struct PlayerLivesChangedEvent
{
	int livesLeft;
};
#endif // !GAME_EVENTS_H

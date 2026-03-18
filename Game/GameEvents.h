#ifndef GAMEEVENTS_H
#define GAMEEVENTS_H

/// Simple holder for event data structs.
// In case this project gets big/needs many events these are to be split up over multiple files.

struct PlayerLivesChangedEvent
{
	int livesLeft;
};

#endif // !GAMEEVENTS_H

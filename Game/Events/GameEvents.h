#ifndef GAME_EVENTS_H
#define GAME_EVENTS_H

/// Simple holder for event data structs.
// In case this project gets big/needs many events these are to be split up over multiple files.

struct TankDeathEvent
{
	int TankId{-1}; // -1 for AI tanks
	int KillerId{-1}; // -1 if unknown
	int ScoreValue{};
};

enum class GameEvent
{
	PlayerDied,
	EnemyKilled,

	StartPlaying,
	scoreSaved,
};
#endif // !GAME_EVENTS_H

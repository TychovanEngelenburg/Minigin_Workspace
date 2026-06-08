#ifndef GAME_EVENTS_H
#define GAME_EVENTS_H

#include <optional>
/// Simple holder for event data structs.
// In case this project gets big/needs many events these are to be split up over multiple files.

struct TankDeathEvent
{
	std::optional<int> PlayerVictim{ std::nullopt };
	std::optional<int> KillingPlayer{ std::nullopt };
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

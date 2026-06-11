#ifndef GAME_EVENTS_H
#define GAME_EVENTS_H

#include <optional>

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

	Continue,
};
#endif // !GAME_EVENTS_H

#ifndef TANK_PRESETS_H
#define TANK_PRESETS_H
#include "Game/Tank/Bullet/BulletConfig.h"
#include "Game/Player/PlayerInputConfig.h"

#include <Minigin/Config/SpriteConfig.h>

#include <optional>
#include <glm/vec2.hpp>
#include <filesystem>

inline const mg::SpriteSheet TankSheet{ "T_SpriteSheet_BattleTanks.png" , 4, 4 };

enum class Allignment
{
	Good,
	Evil
};

struct BarrelConfig
{
	BulletConfig BulletConf;
	bool AimWithMoveDir{ true };
	float ShootInterval{ 1.f };
	float Length{};
	std::optional<mg::SpriteConfig> Sprite{std::nullopt};
};

struct TankStats
{
	float MoveSpeed{ 50.f };
	int MaxHealth{ 1 };
	int CollisionDamage{ 1 };
};

struct TankConfig
{
	std::string Name;
	mg::SpriteConfig Sprite{ TankSheet, { 3, 2 } };
	Allignment Team{Allignment::Evil};
	TankStats Stats{};

	std::optional<BarrelConfig> Barrel{std::nullopt};
};

namespace TankPresets
{
	TankConfig Player(int playerId);
	TankConfig BasicEnemy();
	TankConfig Recogniser();
}

#endif // !TANK_PRESETS_H

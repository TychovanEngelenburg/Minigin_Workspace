#ifndef TANK_PRESETS_H
#define TANK_PRESETS_H

#include "Game/Config/BulletConfig.h"
#include "Game/Config/CollisionConfig.h"
#include "Game/Config/FileLocations.h"

#include <Minigin/Rendering/SpriteConfig.h>

#include <optional>
#include <filesystem>

inline  mg::SpriteSheet const TankSheet { FileLocations::EntitySpriteSheet , 4, 4 };

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
	int KillScore{ 100 };
};

struct TankConfig
{
	std::string Name;
	mg::SpriteConfig Sprite{ TankSheet, { 3, 2 } };
	CollisionConfig Collisions{};
	TankStats Stats{};

	std::optional<BarrelConfig> Barrel{std::nullopt};
};

// TODO: Load these from a file
namespace TankPresets
{
	TankConfig Player(int playerId);
	TankConfig BasicEnemy();
	TankConfig Recogniser();
}

#endif // !TANK_PRESETS_H

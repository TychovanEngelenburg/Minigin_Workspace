#include "TankConfig.h"

TankConfig TankPresets::Player(int playerId)
{
	TankConfig cfg{};
	cfg.Name = "Player";
	cfg.Team = Allignment::Good;

	cfg.Sprite.SheetPos = { 0 + playerId, 3 };

	cfg.Stats.MoveSpeed = 75.f;
	cfg.Stats.MaxHealth = 3;
	cfg.Stats.CollisionDamage = 1;

	BarrelConfig barrel{};
	barrel.AimWithMoveDir = false;
	barrel.ShootInterval = 0.35f;
	barrel.Length = 12.f;

	mg::SpriteConfig barrelSprite{TankSheet};
	barrelSprite.SheetPos = { 0, 2 };  
	barrelSprite.RenderPivot = {6.f, 6.f};
	barrel.Sprite = barrelSprite;

	barrel.BulletConf.Speed = 180.f;
	barrel.BulletConf.Damage = 1;
	barrel.BulletConf.ColliderSize = { 8.f, 8.f };
	barrel.BulletConf.Sprite.SheetPos = { 1, 1 };
	cfg.Barrel = barrel;


	return cfg;
}

TankConfig TankPresets::BasicEnemy()
{
	TankConfig cfg{};
	cfg.Name = "BasicEnemy";
	cfg.Team = Allignment::Evil;

	cfg.Sprite.SheetPos = { 2, 3 };

	cfg.Stats.MoveSpeed = 45.f;
	cfg.Stats.MaxHealth = 1;
	cfg.Stats.CollisionDamage = 1;

	BarrelConfig barrel{};
	barrel.AimWithMoveDir = true;
	barrel.ShootInterval = 1.0f;
	barrel.Length = 10.f;

	barrel.BulletConf.Speed = 120.f;
	barrel.BulletConf.Damage = 1;
	cfg.Barrel = barrel;

	return cfg;
}

TankConfig TankPresets::Recogniser()
{
	TankConfig cfg{};
	cfg.Name = "Recogniser";
	cfg.Team = Allignment::Evil;

	cfg.Sprite.SheetPos = { 3, 3 };

	cfg.Stats.MoveSpeed = 90.f;
	cfg.Stats.MaxHealth = 2;
	cfg.Stats.CollisionDamage = 2;

	BarrelConfig barrel{};
	barrel.AimWithMoveDir = true;
	barrel.ShootInterval = 0.6f;
	barrel.Length = 10.f;

	barrel.BulletConf.Speed = 220.f;
	barrel.BulletConf.Damage = 1;
	cfg.Barrel = barrel;

	return cfg;
}
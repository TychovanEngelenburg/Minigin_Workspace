#include "TankConfig.h"



/// <summary>
/// Components
/// 
/// PlayerTank:
/// Allignment: Good, can only hit/shoot Evil tanks
/// 
/// Base
///		- Sprite
///			Set player sprite
///		- TankVisuals  (Sets/Flips sprite to face movedirection)
///		- Hitbox
///		- CollisionDamage
///		- TankMovement
///			Set player movespeed
///		- TankHealth
///			Set player health
/// 
/// 
/// Barrel (parented to base)
///		- TankBarrel
///			Set barrel shoot offset to barrel length
///			Set bullet config to playerbullet
///		
/// BarrelVisuals (Parented to barrel
///		- Sprite
///			Set barrel sprite
/// 
/// Player can bind tankmovement, barrelturning and shooting.
/// 
/// 
/// 
/// EnemyTank:
/// Allignment: Evil, can only hit/shoot Good tanks
/// 
/// Base
///		- Sprite
///			Set enemytank sprite
///		- TankVisuals  (Sets/Flips sprite to face movedirection)
///		- Hitbox
///		- CollisionDamage
///		- TankMovement
///			Set EnemyTank movespeed
///		- TankHealth
///			Set EnemyTank health
///		- MovementAI (unless player bound to tank) (Handles movement direction)
/// 
/// 
/// Barrel (parented to base)
///		- TankBarrel
///			Set Barreloffset to 0
///			Set bullet config to enemybullet
///		- BarrelAI (automatically aims in movedirection)
/// 
/// Player can bind tankmovement and shooting.
/// 
/// 
/// 
/// Recogniser:
/// Allignment: Evil, can only hit/shoot Good tanks
/// 
/// Base
///		- Sprite
///			Set recogniser sprite
///		- TankVisuals (Sets/Flips sprite to face movedirection)
///		- Hitbox
///		- CollisionDamage
///			Set recogniser damage
///		- TankMovement
///			Set double EnemyTank movespeed
///		- TankHealth
///			Set Recogniser health
///		- MovementAI (Handles movement direction)
/// 
/// </summary>
/// 
/// 
TankConfig TankPresets::Player(int playerId)
{
	TankConfig cfg{};
	cfg.Name = "Player";
	cfg.Collisions.Layer = static_cast<uint32_t>(GameCollisionLayer::Good);
	cfg.Collisions.LayerMask =
		static_cast<uint32_t>(GameCollisionLayer::Evil) |
		static_cast<uint32_t>(GameCollisionLayer::Bullet);

	cfg.Sprite.SheetPos = { 0 + playerId, 3 };

	cfg.Stats.MoveSpeed = 60.f;
	cfg.Stats.MaxHealth = 1;
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
	barrel.BulletConf.Bounces = 5;
	barrel.BulletConf.ColliderSize = { 8.f, 8.f };
	barrel.BulletConf.Sprite.SheetPos = { 1, 1 };
	barrel.BulletConf.Collisions.Layer = static_cast<uint32_t>(GameCollisionLayer::Bullet);
	barrel.BulletConf.Collisions.LayerMask = static_cast<uint32_t>(GameCollisionLayer::Evil);
	cfg.Barrel = barrel;


	return cfg;
}

TankConfig TankPresets::BasicEnemy()
{
	TankConfig cfg{};
	cfg.Name = "BasicEnemy";

	cfg.Collisions.Layer = static_cast<uint32_t>(GameCollisionLayer::Evil);
	cfg.Collisions.LayerMask =
		static_cast<uint32_t>(GameCollisionLayer::Good) |
		static_cast<uint32_t>(GameCollisionLayer::Bullet);

	cfg.Sprite.SheetPos = { 2, 3 };

	cfg.Stats.MoveSpeed = 60.f;
	cfg.Stats.MaxHealth = 3;
	cfg.Stats.CollisionDamage = 1;

	BarrelConfig barrel{};
	barrel.AimWithMoveDir = true;
	barrel.ShootInterval = 1.0f;
	barrel.Length = 10.f;

	barrel.BulletConf.Speed = 120.f;
	barrel.BulletConf.Damage = 1;
	barrel.BulletConf.Collisions.Layer = static_cast<uint32_t>(GameCollisionLayer::Bullet);
	barrel.BulletConf.Collisions.LayerMask = static_cast<uint32_t>(GameCollisionLayer::Good);
	cfg.Barrel = barrel;

	return cfg;
}

TankConfig TankPresets::Recogniser()
{
	TankConfig cfg{};
	cfg.Name = "Recogniser";
	
	cfg.Collisions.Layer = static_cast<uint32_t>(GameCollisionLayer::Evil);
	cfg.Collisions.LayerMask =
		static_cast<uint32_t>(GameCollisionLayer::Good) |
		static_cast<uint32_t>(GameCollisionLayer::Bullet);

	cfg.Sprite.SheetPos = { 3, 3 };

	cfg.Stats.MoveSpeed = 90.f;
	cfg.Stats.MaxHealth = 2;
	cfg.Stats.CollisionDamage = 2;

	return cfg;
}
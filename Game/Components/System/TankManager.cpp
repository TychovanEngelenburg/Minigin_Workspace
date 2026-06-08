#include "TankManager.h"

#include "Game/Core/GameContext.h"
#include "Game/Events/GameEvents.h"

#include "Game/Components/System/GameGrid.h"

#include "Game/Config/TankConfig.h"
#include "Game/Config/PlayerInputConfig.h"

#include "Game/Components/DamageOnCollision.h"
#include "Game/Components/Tank/TankHealth.h"
#include "Game/Components/Tank/TankMovement.h"
#include "Game/Components/Tank/TankBarrel.h"
#include "Game/Components/Tank/RotateWithTank.h"

#include "Game/Commands/MoveTankCommand.h"
#include "Game/Commands/TurnBarrelCommand.h"
#include "Game/Commands/ShootCommand.h"

#include "Game/Components/Enemy/AIMovement.h"
#include "Game/Components/Enemy/AutoShooting.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/Input/InputBinding.h>
#include <Minigin/Input/InputCodes.h>
#include <Minigin/Components/Sprite.h>
#include <Minigin/Collisions/BoxCollider2D.h>
#include <Minigin/Scene/Scene.h>
#include <Minigin/Input/SceneInput.h>
#include <Minigin/Input/InputServiceLocator.h>

#include <cassert>



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
mg::GameObject* TankManager::SpawnTank(glm::ivec2 const& gridPos, TankConfig const& tankConfig, std::optional<PlayerInputConfig> inputConfig)
{
	assert(Object()->Scene() && "SpawnTank cannot be called before object was added to a scene!");

	auto& scene = *Object()->Scene();
	glm::vec2 spriteSize{};


	// Base Tank
	auto tankObj = std::make_unique<mg::GameObject>(tankConfig.Name);

	auto& tankMovement = tankObj->AddComponent<TankMovement>(*m_pGrid);
	tankMovement.SetMoveSpeed(tankConfig.Stats.MoveSpeed);

	auto& tankHealth = tankObj->AddComponent<TankHealth>();
	tankHealth.MaxHealth = tankConfig.Stats.MaxHealth;
	tankHealth.SetScoreValue(tankConfig.Stats.KillScore);
	if (inputConfig.has_value())
	{
		tankHealth.SetPlayerId(inputConfig->PlayerIndex);
	}
	tankHealth.AddListener(&GameContext::Instance());

	tankObj->Transform().SetWorldPosition(m_pGrid->GridToWorld(gridPos));

	// Tank visual
	auto tankVisualsObj = std::make_unique<mg::GameObject>("TankVisuals");
	{
		auto& tankSprite = tankVisualsObj->AddComponent<mg::Sprite>(tankConfig.Sprite.Sheet);
		tankSprite.SetSprite(tankConfig.Sprite.SheetPos);
		spriteSize = tankSprite.Size();
		tankSprite.SetPivot(glm::vec2(spriteSize / 2.f));

		auto& tankRotation = tankVisualsObj->AddComponent<RotateWithTank>();
		tankRotation.SetTank(&tankMovement);

		tankVisualsObj->Transform().SetLocalPosition(glm::vec2(spriteSize / 2.f));
		tankVisualsObj->Transform().SetParent(&tankObj->Transform());
	}

	auto& collider = tankObj->AddComponent<mg::BoxCollider2D>();
	collider.SetSize(spriteSize);
	collider.CollisionLayer = tankConfig.Collisions.Layer;
	collider.CollisionMask = tankConfig.Collisions.LayerMask;

	auto& collisionDamage = tankObj->AddComponent<DamageOnCollision>();
	collisionDamage.SetDamageAmount(tankConfig.Stats.CollisionDamage);

	if (inputConfig.has_value())
	{
		tankObj->Name += "_Player" + std::to_string(inputConfig->PlayerIndex);
		collisionDamage.SetPlayerId(inputConfig->PlayerIndex);
	}

	// Barrel (optional)
	std::unique_ptr<mg::GameObject> barrelObj{};
	std::unique_ptr<mg::GameObject> barrelVisualsObj{};

	bool hasBarrel{ tankConfig.Barrel.has_value() };
	if (hasBarrel)
	{
		barrelObj = std::make_unique<mg::GameObject>("Barrel");

		auto& barrel = barrelObj->AddComponent<TankBarrel>(*m_pGrid);
		barrel.SetBulletPool(m_pBulletPool);
		barrel.SetBarrelLength(tankConfig.Barrel->Length);
		barrel.SetBulletConfig(tankConfig.Barrel->BulletConf);
		barrel.SetCooldown(tankConfig.Barrel->ShootInterval);

		if (inputConfig.has_value())
		{
			barrel.SetPlayerId(inputConfig->PlayerIndex);
		}

		barrelObj->Transform().SetParent(&tankObj->Transform());
		barrelObj->Transform().SetLocalPosition(spriteSize / 2.f);

		if (tankConfig.Barrel->AimWithMoveDir)
		{
			auto& barrelRotation = barrelObj->AddComponent<RotateWithTank>();
			barrelRotation.SetTank(&tankMovement);
		}

		// Barrel visual (optional)
		if (tankConfig.Barrel->Sprite.has_value())
		{
			barrelVisualsObj = std::make_unique<mg::GameObject>("BarrelVisual");

			auto& barrelSprite = barrelVisualsObj->AddComponent<mg::Sprite>(tankConfig.Barrel->Sprite->Sheet);
			barrelSprite.SetSprite(tankConfig.Barrel->Sprite->SheetPos);
			barrelSprite.SetPivot(tankConfig.Barrel->Sprite->RenderPivot);

			barrelVisualsObj->Transform().SetParent(&barrelObj->Transform());
		}
	}

	// Player bindings
	if (inputConfig.has_value())
	{
		bool playerBarrelTurning{ hasBarrel && !tankConfig.Barrel->AimWithMoveDir };

		auto gamepadSlot = m_deviceMapper.GamepadIndexForPlayer(inputConfig->PlayerIndex);
		if (gamepadSlot.has_value())
		{
			BindGamepad(*tankObj, barrelObj.get(), playerBarrelTurning, *gamepadSlot);
		}

		if (m_deviceMapper.PlayerUsesKeyboard(inputConfig->PlayerIndex))
		{
			BindKeyboard(*tankObj.get(), barrelObj.get(), playerBarrelTurning);
		}
	}
	else // AI Controlling
	{
		tankObj->AddComponent<EnemyBehaviour>(*m_pGrid);

		if (barrelObj)
		{
			barrelObj->AddComponent<AutoShooting>(*this, *m_pGrid);
		}
	}

	// Scene adding
	auto tankBase = tankObj.get();

	m_pTanks.push_back(tankBase);

	scene.Add(std::move(tankObj));
	scene.Add(std::move(tankVisualsObj));

	if (barrelObj)
	{
		scene.Add(std::move(barrelObj));
		if (barrelVisualsObj)
		{
			scene.Add(std::move(barrelVisualsObj));
		}
	}

	return tankBase;
}


void TankManager::SetBulletPool(BulletPool* pool)
{
	m_pBulletPool = pool;
	m_pTanks.erase(std::remove(m_pTanks.begin(), m_pTanks.end(), nullptr), m_pTanks.end());

	for (auto* tank : m_pTanks)
	{
		// TODO: This is very heavy code, try to handle it another way.
		for (size_t i = 0; i < tank->Transform().ChildCount(); i++)
		{
			if (auto* barrel = tank->Transform().GetChildAt(i)->Object().GetComponent<TankBarrel>())
			{
				barrel->SetBulletPool(pool);
				break;
			}
		}
	}
}


TankManager::SpawnCounts TankManager::Initialize(GameContext::GameMode const& mode)
{
	assert(Object()->Scene() && "TankManager must be attached to a scene before initializing!");
	m_deviceMapper.Resolve(mg::InputServiceLocator::Fetch());

	auto& playerSpawns = m_pGrid->PlayerSpawnpoints();
	auto& enemySpawns = m_pGrid->EnemySpawnpoints();

	assert(!playerSpawns.empty() && "Level has no player spawn points.");
	assert(!enemySpawns.empty() && "Level has no enemy spawn points.");

	TankManager::SpawnCounts counts{};

	// Spawn players
	SpawnTank(playerSpawns[0], TankPresets::Player(0), PlayerInputConfig(0));
	++counts.Players;

	switch (mode)
	{
		case GameContext::GameMode::Singleplayer:
		case GameContext::GameMode::Coop:
		{
			for (int i{ 1 }; i < playerSpawns.size(); i++)
			{
				SpawnTank(playerSpawns[i], TankPresets::Player(i), PlayerInputConfig(i));
				++counts.Players;
			}
			break;
		}

		case GameContext::GameMode::Versus:
		{
			for (int i{ 1 }; i < playerSpawns.size(); i++)
			{
				SpawnTank(playerSpawns[i], TankPresets::BasicEnemy(), PlayerInputConfig(i));
				++counts.Enemies;
			}
			break;
		}

		default:
		{
			break;
		}
	}

	// Spawn enemies
	for (auto const& enemySpawn : enemySpawns)
	{
		SpawnTank(enemySpawn, TankPresets::BasicEnemy());
		++counts.Enemies;
	}

	return counts;
}

TankManager::TankManager(mg::GameObject& owner, GameGrid& grid)
	: Component(owner)
	, m_pGrid(&grid)
{

}

void TankManager::BindKeyboard(mg::GameObject& playerObj, mg::GameObject* barrelObj, bool canTurnBarrel)
{
	auto& sceneInput{ m_pGrid->Object()->Scene()->InputSystem() };

	auto moveLeft = std::make_unique<mg::InputBinding>(
		0, static_cast<int>(mg::Keycodes::KeyboardKey::Left), mg::InputBinding::DeviceType::Keyboard,
		std::make_unique<MoveTankCommand>(playerObj, TankMovement::Direction::Left), mg::InputBinding::TriggerType::Held
	);
	sceneInput.AddBinding(std::move(moveLeft));

	auto moveUp = std::make_unique<mg::InputBinding>(
		0, static_cast<int>(mg::Keycodes::KeyboardKey::Up), mg::InputBinding::DeviceType::Keyboard,
		std::make_unique<MoveTankCommand>(playerObj, TankMovement::Direction::Up), mg::InputBinding::TriggerType::Held
	);
	sceneInput.AddBinding(std::move(moveUp));

	auto moveRight = std::make_unique<mg::InputBinding>(
		0, static_cast<int>(mg::Keycodes::KeyboardKey::Right), mg::InputBinding::DeviceType::Keyboard,
		std::make_unique<MoveTankCommand>(playerObj, TankMovement::Direction::Right), mg::InputBinding::TriggerType::Held
	);
	sceneInput.AddBinding(std::move(moveRight));

	auto moveDown = std::make_unique<mg::InputBinding>(
		0, static_cast<int>(mg::Keycodes::KeyboardKey::Down), mg::InputBinding::DeviceType::Keyboard,
		std::make_unique<MoveTankCommand>(playerObj, TankMovement::Direction::Down), mg::InputBinding::TriggerType::Held
	);
	sceneInput.AddBinding(std::move(moveDown));




	if (barrelObj)
	{
		auto shoot = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::KeyboardKey::LCtrl), mg::InputBinding::DeviceType::Keyboard,
			std::make_unique<ShootCommand>(*barrelObj), mg::InputBinding::TriggerType::Released
		);
		sceneInput.AddBinding(std::move(shoot));

		if (canTurnBarrel)
		{
			auto turnLeft = std::make_unique<mg::InputBinding>(
				0, static_cast<int>(mg::Keycodes::KeyboardKey::Z), mg::InputBinding::DeviceType::Keyboard,
				std::make_unique<TurnBarrelCommand>(*barrelObj, -90.f), mg::InputBinding::TriggerType::Held
			);
			sceneInput.AddBinding(std::move(turnLeft));

			auto turnRight = std::make_unique<mg::InputBinding>(
				0, static_cast<int>(mg::Keycodes::KeyboardKey::X), mg::InputBinding::DeviceType::Keyboard,
				std::make_unique<TurnBarrelCommand>(*barrelObj, 90.f), mg::InputBinding::TriggerType::Held
			);
			sceneInput.AddBinding(std::move(turnRight));
		}
	}
}

void TankManager::BindGamepad(mg::GameObject& playerObj, mg::GameObject* barrelObj, bool canTurnBarrel, int playerId)
{
	assert(playerId < 4);

	auto& sceneInput{ m_pGrid->Object()->Scene()->InputSystem() };

	auto moveLeft = std::make_unique<mg::InputBinding>(
		playerId, static_cast<int>(mg::Keycodes::GamepadButton::DPadLeft), mg::InputBinding::DeviceType::Gamepad,
		std::make_unique<MoveTankCommand>(playerObj, TankMovement::Direction::Left), mg::InputBinding::TriggerType::Held
	);
	sceneInput.AddBinding(std::move(moveLeft));

	auto moveUp = std::make_unique<mg::InputBinding>(
		playerId, static_cast<int>(mg::Keycodes::GamepadButton::DPadUp), mg::InputBinding::DeviceType::Gamepad,
		std::make_unique<MoveTankCommand>(playerObj, TankMovement::Direction::Up), mg::InputBinding::TriggerType::Held
	);
	sceneInput.AddBinding(std::move(moveUp));

	auto moveRight = std::make_unique<mg::InputBinding>(
		playerId, static_cast<int>(mg::Keycodes::GamepadButton::DPadRight), mg::InputBinding::DeviceType::Gamepad,
		std::make_unique<MoveTankCommand>(playerObj, TankMovement::Direction::Right), mg::InputBinding::TriggerType::Held
	);
	sceneInput.AddBinding(std::move(moveRight));

	auto moveDown = std::make_unique<mg::InputBinding>(
		playerId, static_cast<int>(mg::Keycodes::GamepadButton::DPadDown), mg::InputBinding::DeviceType::Gamepad,
		std::make_unique<MoveTankCommand>(playerObj, TankMovement::Direction::Down), mg::InputBinding::TriggerType::Held
	);
	sceneInput.AddBinding(std::move(moveDown));

	if (barrelObj)
	{
		auto shoot = std::make_unique<mg::InputBinding>(
			playerId, static_cast<int>(mg::Keycodes::GamepadButton::X), mg::InputBinding::DeviceType::Gamepad,
			std::make_unique<ShootCommand>(*barrelObj), mg::InputBinding::TriggerType::Released
		);
		sceneInput.AddBinding(std::move(shoot));


		if (canTurnBarrel)
		{
			auto turnLeft = std::make_unique<mg::InputBinding>(
				playerId, static_cast<int>(mg::Keycodes::GamepadButton::A), mg::InputBinding::DeviceType::Gamepad,
				std::make_unique<TurnBarrelCommand>(*barrelObj, -90.f), mg::InputBinding::TriggerType::Held
			);
			sceneInput.AddBinding(std::move(turnLeft));

			auto turnRight = std::make_unique<mg::InputBinding>(
				playerId, static_cast<int>(mg::Keycodes::GamepadButton::B), mg::InputBinding::DeviceType::Gamepad,
				std::make_unique<TurnBarrelCommand>(*barrelObj, 90.f), mg::InputBinding::TriggerType::Held
			);
			sceneInput.AddBinding(std::move(turnRight));
		}
	}
}
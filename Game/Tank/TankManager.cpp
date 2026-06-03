#include "TankManager.h"

#include "Game/Commands/MoveTankCommand.h"
#include "Game/Commands/TurnBarrelCommand.h"
#include "Game/Commands/ShootCommand.h"

#include "Game/Tank/TankHealth.h"
#include "Game/Tank/TankMovement.h"
#include "Game/Tank/TankBarrel.h"
#include "Game/Tank/TankVisuals.h"
#include "Game/Tank/RotateWithTank.h"

#include "Game/Grid/GameGrid.h"

#include "Game/Enemy/EnemyBehaviour.h"
#include "Game/Enemy/AutoShooting.h"

#include "Game/Config/TankConfig.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/InputHandling/InputBinding.h>
#include <Minigin/InputHandling/InputCodes.h>
#include <Minigin/EngineComponents/Sprite.h>
#include <Minigin/CollisionSystem/BoxCollider2D.h>
#include <Minigin/Scene/Scene.h>
#include <Minigin/InputHandling/SceneInput.h>

TankManager::TankManager(mg::GameObject& owner, GameGrid& grid)
	: Component(owner)
	, m_pGrid{ &grid }
{

}


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

	auto& scene = *m_pGrid->Owner()->Scene();

	glm::vec2 spriteSize{};


	// Base Tank
	auto tankObj = std::make_unique<mg::GameObject>(tankConfig.Name);
	auto& tankMovement = tankObj->AddComponent<TankMovement>(*m_pGrid);
	tankMovement.SetMoveSpeed(tankConfig.Stats.MoveSpeed);
	{
		if (inputConfig)
		{
			tankObj->Name += "_Player" + std::to_string(inputConfig->PlayerIndex);
		}

		/*auto& health = */ tankObj->AddComponent<TankHealth>();
		//health.SetMaxHealth(tankConfig.Stats.MaxHealth);

		tankObj->Transform().SetWorldPosition(m_pGrid->GridToWorld(gridPos));
	}

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

	// Barrel
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

		barrelObj->Transform().SetParent(&tankObj->Transform());
		barrelObj->Transform().SetLocalPosition(spriteSize / 2.f);

		if (tankConfig.Barrel->AimWithMoveDir)
		{
			auto& barrelRotation = barrelObj->AddComponent<RotateWithTank>();
			barrelRotation.SetTank(&tankMovement);
		}

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
		bool playerBarrelTurning{ hasBarrel ? !tankConfig.Barrel->AimWithMoveDir : false };
		
		BindGamepad(*tankObj.get(), barrelObj.get(), inputConfig->PlayerIndex, playerBarrelTurning);

		if (inputConfig->PlayerIndex == 0)
		{
			BindKeyboard(*tankObj.get(), barrelObj.get(), playerBarrelTurning);
		}
	}
	else
	{
		tankObj->AddComponent<EnemyBehaviour>(*m_pGrid);
		barrelObj->AddComponent<AutoShooting>(*this, *m_pGrid);
	}

	auto result = tankObj.get();

	m_pTanks.push_back(result);

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

	return result;
}


void TankManager::SetBulletPool(BulletPool* pool)
{
	m_pBulletPool = pool;
	m_pTanks.erase(std::remove(m_pTanks.begin(), m_pTanks.end(), nullptr), m_pTanks.end());

	for (auto* tank : m_pTanks)
	{
		if (auto* barrel = tank->Transform().GetChildAt(0)->Owner().GetComponent<TankBarrel>())
		{
			barrel->SetBulletPool(pool);
		}
	}
}

void TankManager::Awake()
{
	auto& playerSpawns{ m_pGrid->PlayerSpawnpoints() };

	for (size_t i = 0; i < playerSpawns.size(); i++)
	{
		SpawnTank(playerSpawns[i], TankPresets::Player(static_cast<int>(i)), PlayerInputConfig(static_cast<int>(i)));
	}

	auto& enemySpawns{ m_pGrid->EnemySpawnpoints() };

	for (size_t i = 0; i < enemySpawns.size(); i++)
	{
		SpawnTank(enemySpawns[i], TankPresets::BasicEnemy());
	}
}

void TankManager::BindKeyboard(mg::GameObject& playerObj, mg::GameObject* barrelObj, bool canTurnBarrel)
{
	auto& sceneInput{ Owner()->Scene()->InputSystem() };

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

	auto& sceneInput{ m_pGrid->Owner()->Scene()->InputSystem() };

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
			0, static_cast<int>(mg::Keycodes::GamepadButton::X), mg::InputBinding::DeviceType::Gamepad,
			std::make_unique<ShootCommand>(*barrelObj), mg::InputBinding::TriggerType::Released
		);
		sceneInput.AddBinding(std::move(shoot));


		if (canTurnBarrel)
		{
			auto turnLeft = std::make_unique<mg::InputBinding>(
				0, static_cast<int>(mg::Keycodes::GamepadButton::A), mg::InputBinding::DeviceType::Gamepad,
				std::make_unique<TurnBarrelCommand>(*barrelObj, -90.f), mg::InputBinding::TriggerType::Held
			);
			sceneInput.AddBinding(std::move(turnLeft));

			auto turnRight = std::make_unique<mg::InputBinding>(
				0, static_cast<int>(mg::Keycodes::GamepadButton::B), mg::InputBinding::DeviceType::Gamepad,
				std::make_unique<TurnBarrelCommand>(*barrelObj, 90.f), mg::InputBinding::TriggerType::Held
			);
			sceneInput.AddBinding(std::move(turnRight));
		}
	}
}
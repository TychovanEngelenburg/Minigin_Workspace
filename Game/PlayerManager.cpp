#include "PlayerManager.h"

#include "Commands/MoveTankCommand.h"

#include "Tank/TankHealth.h"
#include "Tank/TankMovement.h"
#include "Tank/Bullet/BulletMovement.h"

#include "Grid/GameGrid.h"

#include "DamageOnCollision.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/InputHandling/InputBinding.h>
#include <Minigin/InputHandling/InputCodes.h>
#include <Minigin/EngineComponents/Sprite.h>
#include <Minigin/CollisionSystem/BoxCollider2D.h>
#include <Minigin/Scene/Scene.h>

PlayerManager::PlayerManager(mg::GameObject& owner, GameGrid& grid)
	: Component(owner)
	, m_pGrid{ &grid }
{

}

void PlayerManager::SpawnPlayer(int playerId)
{
	auto& sceneOut{ *Owner()->Scene() };

	auto player = std::make_unique<mg::GameObject>("Player_" + std::to_string(playerId), glm::vec3(20, 120.f, 0.f));
	{
		auto& sprite = player->AddComponent<mg::Sprite>("T_SpriteSheet_Tron.png", mg::SpriteSheet{ 13, 5 });
		sprite.SetSprite(8, 0);

		auto& hitBox{ player->AddComponent<mg::BoxCollider2D>() };
		hitBox.SetSize(sprite.Size());

		player->AddComponent<TankHealth>();
		player->AddComponent<TankMovement>(*m_pGrid, 100.f);
	}

	if (playerId == 0)
	{
		BindKeyboard(player.get());
	}

	BindGamepad(player.get(), playerId);

	sceneOut.Add(std::move(player));
}

void PlayerManager::Awake()
{
	auto& playerSpawns{ m_pGrid->PlayerSpawnpoints() };

	for (int i = 0; i < playerSpawns.size(); i++)
	{
		SpawnPlayer(i);
	}
}

void PlayerManager::BindKeyboard(mg::GameObject* playerObj)
{
	auto& scenInput{ Owner()->Scene()->InputSystem() };

	auto moveLeft = std::make_unique<mg::InputBinding>(
		0, static_cast<int>(mg::Keycodes::KeyboardKey::A), mg::InputBinding::DeviceType::Keyboard,
		std::make_unique<MoveTankCommand>(playerObj, TankMovement::Direction::Left), mg::InputBinding::TriggerType::Held
	);
	scenInput.AddBinding(std::move(moveLeft));

	auto moveUp = std::make_unique<mg::InputBinding>(
		0, static_cast<int>(mg::Keycodes::KeyboardKey::W), mg::InputBinding::DeviceType::Keyboard,
		std::make_unique<MoveTankCommand>(playerObj, TankMovement::Direction::Up), mg::InputBinding::TriggerType::Held
	);
	scenInput.AddBinding(std::move(moveUp));

	auto moveRight = std::make_unique<mg::InputBinding>(
		0, static_cast<int>(mg::Keycodes::KeyboardKey::D), mg::InputBinding::DeviceType::Keyboard,
		std::make_unique<MoveTankCommand>(playerObj, TankMovement::Direction::Right), mg::InputBinding::TriggerType::Held
	);
	scenInput.AddBinding(std::move(moveRight));

	auto moveDown = std::make_unique<mg::InputBinding>(
		0, static_cast<int>(mg::Keycodes::KeyboardKey::S), mg::InputBinding::DeviceType::Keyboard,
		std::make_unique<MoveTankCommand>(playerObj, TankMovement::Direction::Down), mg::InputBinding::TriggerType::Held
	);
	scenInput.AddBinding(std::move(moveDown));
}

void PlayerManager::BindGamepad(mg::GameObject* playerObj, int playerId)
{
	assert(playerId < 4);

	auto& sceneInput{ Owner()->Scene()->InputSystem() };

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
}

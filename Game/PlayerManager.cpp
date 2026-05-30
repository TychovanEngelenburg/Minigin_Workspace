#include "PlayerManager.h"

#include "Commands/MoveTankCommand.h"

#include "Tank/TankHealth.h"
#include "Tank/TankMovement.h"
#include "Tank/TankBarrel.h"

#include "Grid/GameGrid.h"

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

	mg::GameObject* PlayerManager::SpawnPlayer(int playerId, glm::ivec2 gridPos)
	{
		auto& sceneOut{ *Owner()->Scene() };

		glm::vec2 playerSize{};
		auto player = std::make_unique<mg::GameObject>("Player_" + std::to_string(playerId));
		{
			player->Transform().SetWorldPosition(m_pGrid->GridToWorld(gridPos));

			auto& sprite = player->AddComponent<mg::Sprite>("T_SpriteSheet_BattleTanks.png", mg::SpriteSheet{ 4, 5 });
			sprite.SetSprite(2 * playerId, 3);
			playerSize = sprite.Size();

			auto& hitBox{ player->AddComponent<mg::BoxCollider2D>() };
			hitBox.SetSize(playerSize);

			player->AddComponent<TankHealth>();
			player->AddComponent<TankMovement>(*m_pGrid);
		}

		auto barrel = std::make_unique<mg::GameObject>("barrel");
		{
			barrel->Transform().SetLocalPosition(playerSize / 2.f);

			auto& sprite = barrel->AddComponent<mg::Sprite>("T_SpriteSheet_BattleTanks.png", mg::SpriteSheet{ 4, 5 });
			sprite.SetSprite(0, 2);

			barrel->AddComponent<TankBarrel>();

			barrel->Transform().SetParent(&player->Transform());
		}



		if (playerId == 0)
		{
			BindKeyboard(player.get());
		}

		BindGamepad(player.get(), playerId);

		auto pPlayer{ player.get() };
		m_pPlayers.push_back(pPlayer);

		sceneOut.Add(std::move(player));
		sceneOut.Add(std::move(barrel));
		
		return pPlayer;
	}

void PlayerManager::Awake()
{
	auto& playerSpawns{ m_pGrid->PlayerSpawnpoints() };

	for (int i = 0; i < playerSpawns.size(); i++)
	{
		SpawnPlayer(i, playerSpawns[i]);
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

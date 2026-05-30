#include "SceneLoading.h"

// Engine
#include <Minigin/Scene/Scene.h>
#include <Minigin/Scene/GameObject.h>
#include <Minigin/EngineComponents/TextComponent.h>
#include <Minigin/EngineComponents/Sprite.h>
#include <Minigin/CollisionSystem/BoxCollider2D.h>
#include <Minigin/SoundSystem/SoundServiceLocator.h>
#include <Minigin/SoundSystem/ISoundSystem.h>

// Input
#include <Minigin/InputHandling/InputBinding.h>
#include <Minigin/InputHandling/InputCodes.h>

// Commands
#include "Commands/StartGameCommand.h"

// UI
#include "UI/FPS_UI.h"
#include "UI/PlayerHealth_UI.h"

// Game
#include "Grid/GameGrid.h"
#include "PlayerManager.h"



// temp
#include "Tank/TankHealth.h"
#include "Tank/TankMovement.h"
#include "Tank/TankBarrel.h"
#include "Enemy/EnemyBehaviour.h"

void SceneLoading::LoadTestScene(mg::Scene& sceneOut)
{
#pragma region Environment
	auto grid = std::make_unique<mg::GameObject>("Grid", glm::vec3(20, 100.f, 0.f));
	{
		grid->AddComponent<GameGrid>("LevelData/01.lvl", 16.f);
	}

#pragma endregion Environment

	auto playerManager = std::make_unique<mg::GameObject>("PlayerManager");
	{
		playerManager->AddComponent<PlayerManager>(*grid->GetComponent<GameGrid>());
	}

	auto enemy = std::make_unique<mg::GameObject>("Enemy", glm::vec3(20, 120.f, 0.f));
	{
		auto& sprite = enemy->AddComponent<mg::Sprite>("T_SpriteSheet_BattleTanks.png", mg::SpriteSheet{ 4, 5 });
		sprite.SetSprite(0, 4);

		auto& hitBox{ enemy->AddComponent<mg::BoxCollider2D>() };
		hitBox.SetSize(sprite.Size());

		enemy->AddComponent<TankHealth>();

		auto& movement{ enemy->AddComponent<TankMovement>(*grid->GetComponent<GameGrid>()) };
		movement.SetMoveSpeed(50.f);
	
		 enemy->AddComponent<EnemyBehaviour>();
	}

	//auto bullet = std::make_unique<mg::GameObject>("PlayerBullet_", glm::vec3(36.f, 146.f, 0.f));
	//{
	//	bullet->AddComponent<BulletMovement>(grid->GetComponent<GameGrid>());
	//	bullet->AddComponent<DamageOnCollision>(1);

	//	auto& sprite{ bullet->AddComponent<mg::Sprite>("T_SpriteSheet_Tron.png", mg::SpriteSheet(13, 5)) };
	//	sprite.SetSprite(6, 0);

	//	auto& hitBox{ bullet->AddComponent<mg::BoxCollider2D>() };
	//	hitBox.SetSize({ 10.f, 8.f });
	//}

#pragma region UI
	auto fpsCounterUI = std::make_unique<mg::GameObject>("FPS_Counter", glm::vec3(100.f, 0.f, 0.f));
	{
		auto& textComp = fpsCounterUI->AddComponent<mg::TextComponent>("00", "Lingua.otf", 36);
		textComp.SetColor({ 255, 255, 0, 255 });

		fpsCounterUI->AddComponent<FPS_UI>();
	}


	float yPos{ 20 };
	int constexpr textSize{ 18 };
	float constexpr marginSize{ 5.f };

	auto playerOneInfoUI = std::make_unique<mg::GameObject>("Player1_Controls", glm::vec3(marginSize, yPos, 0.f));
	{
		auto& textComp = playerOneInfoUI->AddComponent<mg::TextComponent>("00", "Lingua.otf", textSize);
		textComp.SetColor({ 255, 255, 255, 255 });
		textComp.SetText("Use the D-Pad to move Player 1, X to inflict damage and play a soundeffect"); //, A and B to gain score");
	}

	yPos += textSize + marginSize;
	auto playerTwoInfoUI = std::make_unique<mg::GameObject>("Player1_Controls", glm::vec3(marginSize, yPos, 0.f));
	{
		auto& textComp = playerTwoInfoUI->AddComponent<mg::TextComponent>("00", "Lingua.otf", textSize);
		textComp.SetColor({ 255, 255, 255, 255 });
		textComp.SetText("Use WASD to move Player 2, C to inflict damage and P and play a soundeffect"); //, Z and X to gain score");
	}

	yPos += textSize + marginSize; // White line

	yPos += textSize + marginSize;
	auto playerOneHealthUI = std::make_unique<mg::GameObject>("Player1_Health", glm::vec3(marginSize, yPos, 0.f));
	{
		auto& textComp = playerOneHealthUI->AddComponent<mg::TextComponent>("00", "Lingua.otf", textSize);
		textComp.SetColor({ 255, 255, 255, 255 });

		//auto& healthDisp = playerOneHealthUI->AddComponent<PlayerHealth_UI>();
		//gamepadPlayer->GetComponent<TankHealth>()->AddListener(&healthDisp);
	}

	yPos += textSize + marginSize;
	// Score element here

	yPos += textSize + marginSize;

	auto playerTwoHealthUI = std::make_unique<mg::GameObject>("Player2_Health", glm::vec3(marginSize, 120.f, 0.f));
	{
		auto& textComp = playerTwoHealthUI->AddComponent<mg::TextComponent>("00", "Lingua.otf", textSize);
		textComp.SetColor({ 255, 255, 255, 255 });

		//auto& healthDisp = playerTwoHealthUI->AddComponent<PlayerHealth_UI>();
		//keyboardPlayer->GetComponent<TankHealth>()->AddListener(&healthDisp);
	}

	yPos += textSize + marginSize;
	// Score element here
#pragma endregion UI

	// TODO: Make gameobjects own their children,
	// add a Z-index (decide which child is drawn before which).
	// in general transform needs a complete rework. (which is why I have avoided parenting for now)

	sceneOut.Add(std::move(grid));
	sceneOut.Add(std::move(playerManager));
	sceneOut.Add(std::move(enemy));

	//sceneOut.Add(std::move(gamepadPlayer));
	//sceneOut.Add(std::move(keyboardPlayer));
	//sceneOut.Add(std::move(bullet));

	sceneOut.Add(std::move(fpsCounterUI));
	sceneOut.Add(std::move(playerOneInfoUI));
	sceneOut.Add(std::move(playerTwoInfoUI));
	sceneOut.Add(std::move(playerOneHealthUI));
	sceneOut.Add(std::move(playerTwoHealthUI));


	auto& audioSystem = mg::SoundServiceLocator::Fetch();
	audioSystem.PlayMusic({ "./Data/Audio_Tron1982/03_IO_Tower.wav", "music", -1 });
	audioSystem.SetMusicVolume(.5f);

	sceneOut.Start();
}

void SceneLoading::LoadMainMenuScene(mg::Scene& sceneOut)
{
	auto object = std::make_unique<mg::GameObject>("Header_Text", glm::vec3(150.f, 20.f, 0.f));
	{
		auto& textComp = object->AddComponent<mg::TextComponent>("Press Gamepad A/ Keyboard K to continue!", "Lingua.otf", 36);
		textComp.SetColor({ 255, 255, 0, 255 });
		sceneOut.Add(std::move(object));
	}

	auto startGameInput = std::make_unique<mg::InputBinding>(
		0, static_cast<int>(mg::Keycodes::KeyboardKey::K), mg::InputBinding::DeviceType::Keyboard,
		std::make_unique<StartGameCommand>(), mg::InputBinding::TriggerType::Released
	);
	sceneOut.InputSystem().AddBinding(std::move(startGameInput));

	startGameInput = std::make_unique<mg::InputBinding>(
		0, static_cast<int>(mg::Keycodes::GamepadButton::A), mg::InputBinding::DeviceType::Gamepad,
		std::make_unique<StartGameCommand>(), mg::InputBinding::TriggerType::Released
	);
	sceneOut.InputSystem().AddBinding(std::move(startGameInput));

	sceneOut.Start();
}

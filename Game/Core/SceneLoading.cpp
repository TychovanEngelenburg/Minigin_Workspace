#include "SceneLoading.h"

// Commands
#include "Commands/StartGameCommand.h"

// UI
#include "Game/Components/UI/FPS_UI.h"
#include "Game/Components/UI/PlayerHealth_UI.h"

// Game
#include "Game/Components/System/GameGrid.h"
#include "Game/Components/System/TankManager.h"
#include "Game/Components/System/BulletPool.h"
#include "Game/Components/GameManager.h"

// Engine
#include <Minigin/Scene/Scene.h>
#include <Minigin/Scene/GameObject.h>
#include <Minigin/Components/TextComponent.h>
#include <Minigin/Components/Sprite.h>
#include <Minigin/Collisions/BoxCollider2D.h>
#include <Minigin/Audio/SoundServiceLocator.h>
#include <Minigin/Audio/ISoundSystem.h>

// Input
#include <Minigin/Input/InputBinding.h>
#include <Minigin/Input/InputCodes.h>
#include <Minigin/Input/SceneInput.h>

#include <filesystem>

void SceneLoading::LoadTestScene(mg::Scene&)
{
	//#pragma region Environment
	//	auto grid = std::make_unique<mg::GameObject>("Grid", glm::vec2(20, 100.f));
	//	{
	//		grid->AddComponent<GameGrid>("LevelData/01.lvl", 16.f);
	//	}
	//	auto* pGrid = grid.get();
	//	sceneOut.Add(std::move(grid));
	//
	//#pragma endregion Environment
	//
	//	auto bulletManager = std::make_unique<mg::GameObject>("BulletManager");
	//	{
	//		 bulletManager->AddComponent<BulletPool>(*pGrid->GetComponent<GameGrid>(), 32);
	//	}
	//
	//	auto playerManager = std::make_unique<mg::GameObject>("TankManager");
	//	{
	//		auto& manegerComp = playerManager->AddComponent<TankManager>(*pGrid->GetComponent<GameGrid>());
	//		manegerComp.SetBulletPool(bulletManager->GetComponent<BulletPool>());
	//	}
	//
	//#pragma region UI
	//	auto fpsCounterUI = std::make_unique<mg::GameObject>("FPS_Counter", glm::vec2(100.f, 0.f));
	//	{
	//		auto& textComp = fpsCounterUI->AddComponent<mg::TextComponent>("00", "Lingua.otf", 36);
	//		textComp.SetColor({ 255, 255, 0, 255 });
	//
	//		fpsCounterUI->AddComponent<FPS_UI>();
	//	}
	//
	//
	//	float yPos{ 20 };
	//	int constexpr textSize{ 18 };
	//	float constexpr marginSize{ 5.f };
	//
	//	auto playerOneInfoUI = std::make_unique<mg::GameObject>("Player1_Controls", glm::vec2(marginSize, yPos));
	//	{
	//		auto& textComp = playerOneInfoUI->AddComponent<mg::TextComponent>("00", "Lingua.otf", textSize);
	//		textComp.SetColor({ 255, 255, 255, 255 });
	//		textComp.SetText("Use the D-Pad to move Player 1, X to inflict damage and play a soundeffect"); //, A and B to gain score");
	//	}
	//
	//	yPos += textSize + marginSize;
	//	auto playerTwoInfoUI = std::make_unique<mg::GameObject>("Player1_Controls", glm::vec2(marginSize, yPos));
	//	{
	//		auto& textComp = playerTwoInfoUI->AddComponent<mg::TextComponent>("00", "Lingua.otf", textSize);
	//		textComp.SetColor({ 255, 255, 255, 255 });
	//		textComp.SetText("Use WASD to move Player 2, C to inflict damage and P and play a soundeffect"); //, Z and X to gain score");
	//	}
	//
	//	yPos += textSize + marginSize; // White line
	//
	//	yPos += textSize + marginSize;
	//	auto playerOneHealthUI = std::make_unique<mg::GameObject>("Player1_Health", glm::vec2(marginSize, yPos));
	//	{
	//		auto& textComp = playerOneHealthUI->AddComponent<mg::TextComponent>("00", "Lingua.otf", textSize);
	//		textComp.SetColor({ 255, 255, 255, 255 });
	//
	//		//auto& healthDisp = playerOneHealthUI->AddComponent<PlayerHealth_UI>();
	//		//gamepadPlayer->GetComponent<TankHealth>()->AddListener(&healthDisp);
	//	}
	//
	//	yPos += textSize + marginSize;
	//	// Score element here
	//
	//	yPos += textSize + marginSize;
	//
	//	auto playerTwoHealthUI = std::make_unique<mg::GameObject>("Player2_Health", glm::vec2(marginSize, 120.f));
	//	{
	//		auto& textComp = playerTwoHealthUI->AddComponent<mg::TextComponent>("00", "Lingua.otf", textSize);
	//		textComp.SetColor({ 255, 255, 255, 255 });
	//
	//		//auto& healthDisp = playerTwoHealthUI->AddComponent<PlayerHealth_UI>();
	//		//keyboardPlayer->GetComponent<TankHealth>()->AddListener(&healthDisp);
	//	}
	//
	//	yPos += textSize + marginSize;
	//	// Score element here
	//#pragma endregion UI
	//
	//	// TODO: Make gameobjects own their children,
	//	// add a Z-index (decide which child is drawn before which).
	//	// in general transform needs a complete rework. (which is why I have avoided parenting for now)
	//
	//	sceneOut.Add(std::move(bulletManager));
	//	sceneOut.Add(std::move(playerManager));
	//
	//	sceneOut.Add(std::move(fpsCounterUI));
	//	sceneOut.Add(std::move(playerOneInfoUI));
	//	sceneOut.Add(std::move(playerTwoInfoUI));
	//	sceneOut.Add(std::move(playerOneHealthUI));
	//	sceneOut.Add(std::move(playerTwoHealthUI));
	//
	//
	//	auto& audioSystem = mg::SoundServiceLocator::Fetch();
	//	audioSystem.PlayMusic({ "./Data/Audio_Tron1982/03_IO_Tower.wav", "music", -1 });
	//	audioSystem.SetMusicVolume(.5f);
}

TankManager::SpawnCounts SceneLoading::LoadLevelScene(mg::Scene& sceneOut, std::filesystem::path const& levelFile, GameContext::GameMode mode)
{
	auto gridObj = std::make_unique<mg::GameObject>("Grid", glm::vec2(20, 100.f));
	auto& gridComp = gridObj->AddComponent<GameGrid>(levelFile, 16.f);
	sceneOut.Add(std::move(gridObj));

	auto bulletManagerObj = std::make_unique<mg::GameObject>("BulletManager");
	auto& bulletPoolComp = bulletManagerObj->AddComponent<BulletPool>(gridComp, 32);
	sceneOut.Add(std::move(bulletManagerObj));

	auto tankManagerObj = std::make_unique<mg::GameObject>("TankManager");
	auto& tankManager = tankManagerObj->AddComponent<TankManager>(gridComp);
	tankManager.SetBulletPool(&bulletPoolComp);
	sceneOut.Add(std::move(tankManagerObj));

	TankManager::SpawnCounts counts{ tankManager.Initialize(mode) };

	auto gameManagerObj = std::make_unique<mg::GameObject>("gameManager");
	gameManagerObj->AddComponent<GameManager>();
	sceneOut.Add(std::move(gameManagerObj));

	return counts;
}

void SceneLoading::LoadMainMenuScene(mg::Scene& sceneOut)
{
	// Header
	{
		auto obj = std::make_unique<mg::GameObject>("Header_Text", glm::vec2(150.f, 20.f));
		auto& textComp = obj->AddComponent<mg::TextComponent>("Press Gamepad A / Keyboard CTRL to start!", "Lingua.otf", 36);
		textComp.SetColor({ 255, 255, 0, 255 });
		sceneOut.Add(std::move(obj));
	}

	// Mode display
	{
		auto obj = std::make_unique<mg::GameObject>("Mode_Text", glm::vec2(150.f, 80.f));
		auto& textComp = obj->AddComponent<mg::TextComponent>("Press Tab / Gamepad Y to change mode", "Lingua.otf", 24);
		textComp.SetColor({ 200, 200, 200, 255 });
		sceneOut.Add(std::move(obj));
	}

	// Input
	{
		auto keyboard = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::KeyboardKey::K), mg::InputBinding::DeviceType::Keyboard,
			std::make_unique<StartGameCommand>(), mg::InputBinding::TriggerType::Released
		);
		sceneOut.InputSystem().AddBinding(std::move(keyboard));

		auto gamepad = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::GamepadButton::A), mg::InputBinding::DeviceType::Gamepad,
			std::make_unique<StartGameCommand>(), mg::InputBinding::TriggerType::Released
		);
		sceneOut.InputSystem().AddBinding(std::move(gamepad));

	}

	auto gameManagerObj = std::make_unique<mg::GameObject>("gameManager");
	gameManagerObj->AddComponent<GameManager>();
	sceneOut.Add(std::move(gameManagerObj));
}

void SceneLoading::LoadScoreboardScene(mg::Scene& sceneOut)
{
	// TODO

	auto gameManagerObj = std::make_unique<mg::GameObject>("gameManager");
	gameManagerObj->AddComponent<GameManager>();
	sceneOut.Add(std::move(gameManagerObj));
}

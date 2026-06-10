#include "SceneLoading.h"

// General
#include "Game/Components/GameManager.h"
#include "Game/Commands/ToggleMuteCommand.h"

// Level
#include "Game/Components/UI/HudManager.h"
#include "Game/Components/System/GameGrid.h"
#include "Game/Components/System/TankManager.h"
#include "Game/Components/System/BulletPool.h"

// Main menu
#include "Game/Commands/StartGameCommand.h"

// Engine
#include <Minigin/Scene/Scene.h>
#include <Minigin/Scene/GameObject.h>

#include <Minigin/Components/TextComponent.h>

#include <Minigin/Input/InputBinding.h>
#include <Minigin/Input/InputCodes.h>
#include <Minigin/Input/SceneInput.h>


#include <Minigin/Audio/SoundServiceLocator.h>
#include <Minigin/Audio/ISoundSystem.h>

#include <filesystem>

/// <summary>
/// Apply this to all scenes!
/// </summary>
namespace
{
	void ApplyDefault(mg::Scene& sceneOut)
	{
		auto gameManagerObj = std::make_unique<mg::GameObject>("gameManager");
		gameManagerObj->AddComponent<GameManager>();
		sceneOut.Add(std::move(gameManagerObj));


		auto toggleMuteCommand = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::KeyboardKey::F2), mg::InputBinding::DeviceType::Keyboard,
			std::make_unique<ToggleMuteCommand>(), mg::InputBinding::TriggerType::Released
		);

		sceneOut.InputSystem().AddBinding(std::move(toggleMuteCommand));
	}
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

	auto hudManager = std::make_unique<mg::GameObject>("HUD_Manager");
	hudManager->AddComponent<HUDManager>();
	sceneOut.Add(std::move(hudManager));

	auto& audioSystem = mg::SoundServiceLocator::Fetch();
	audioSystem.PlayMusic({ "./Data/Audio_Tron1982/03_IO_Tower.wav", "music", -1 });
	audioSystem.SetMusicVolume(.5f);

	ApplyDefault(sceneOut);

	return counts;
}

void SceneLoading::LoadMainMenuScene(mg::Scene& sceneOut)
{
	// Header
	{
		auto obj = std::make_unique<mg::GameObject>("Header_Text", glm::vec2(150.f, 20.f));
		auto& textComp = obj->AddComponent<mg::TextComponent>("Lingua.otf", 36);
		textComp.SetText("Press Gamepad A / Keyboard CTRL to start!");
		textComp.SetColor({ 255, 255, 0, 255 });
		sceneOut.Add(std::move(obj));
	}

	// Mode display
	{
		auto obj = std::make_unique<mg::GameObject>("Mode_Text", glm::vec2(150.f, 80.f));
		auto& textComp = obj->AddComponent<mg::TextComponent>("Lingua.otf", 24);
		textComp.SetText("Press Tab / Gamepad Y to change mode");
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
	
	ApplyDefault(sceneOut);
}

void SceneLoading::LoadScoreboardScene(mg::Scene& sceneOut)
{
	// TODO

	ApplyDefault(sceneOut);
}

#include "SceneLoading.h"

// General
#include "Game/Components/GameManager.h"
#include "Game/Commands/ToggleMuteCommand.h"
#include "Game/Commands/ContinueSceneCommand.h"
#include "Game/Config/FileConfig.h"

// Level
#include "Game/Components/UI/HudManager.h"
#include "Game/Components/System/GameGrid.h"
#include "Game/Components/System/TankManager.h"
#include "Game/Components/System/BulletPool.h"
#include "Game/Components/Teleporter/Teleporter.h"
#include "Game/Config/CollisionConfig.h"
// Main menu
#include "Game/Commands/ToggleGameModeCommand.h"
#include "Game/Components/UI/GameModeUI.h"

// Saving
#include "Game/Components/System/ScoreSaver.h"
#include "Game/Components/UI/TotalScoreUI.h"
#include "Game/Commands/ScoreSaving/SaveScoreCommand.h"
#include "Game/Commands/ScoreSaving/ScrolllLetterCommand.h"
#include "Game/Commands/ScoreSaving/CancelCharacterCommand.h"

// ScoreBoard
#include "Game/Components/UI/ScoreBoardUI.h"

// Engine
#include <Minigin/Scene/Scene.h>
#include <Minigin/Scene/GameObject.h>

#include <Minigin/Components/TextComponent.h>
#include <Minigin/Components/Sprite.h>
#include <Minigin/Rendering/SpriteConfig.h>

#include <Minigin/Collisions/BoxCollider2D.h>

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

		// Mute input
		{
			auto toggleMuteCommand = std::make_unique<mg::InputBinding>(
				0, static_cast<int>(mg::Keycodes::KeyboardKey::F2), mg::InputBinding::DeviceType::Keyboard,
				std::make_unique<ToggleMuteCommand>(), mg::InputBinding::TriggerType::Released
			);
			sceneOut.InputSystem().AddBinding(std::move(toggleMuteCommand));
		}

		// Continue command
		{
			auto continueSceneCommand = std::make_unique<mg::InputBinding>(
				0, static_cast<int>(mg::Keycodes::KeyboardKey::F1), mg::InputBinding::DeviceType::Keyboard,
				std::make_unique<ContinueSceneCommand>(), mg::InputBinding::TriggerType::Released
			);
			sceneOut.InputSystem().AddBinding(std::move(continueSceneCommand));
		}
	}
}

TankManager::SpawnCounts SceneLoading::LoadLevelScene(mg::Scene& sceneOut, std::filesystem::path const& levelFile)
{
	// Grid
	auto gridObj = std::make_unique<mg::GameObject>("Grid", glm::vec2(20, 100.f));
	auto& gridComp = gridObj->AddComponent<GameGrid>(levelFile, 16.f);
	sceneOut.Add(std::move(gridObj));

	// Bullet manager
	auto bulletManagerObj = std::make_unique<mg::GameObject>("BulletManager");
	auto& bulletPoolComp = bulletManagerObj->AddComponent<BulletPool>(gridComp, 32);
	sceneOut.Add(std::move(bulletManagerObj));

	// Tank manager
	auto tankManagerObj = std::make_unique<mg::GameObject>("TankManager");
	auto& tankManager = tankManagerObj->AddComponent<TankManager>(gridComp);
	tankManager.SetBulletPool(&bulletPoolComp);
	sceneOut.Add(std::move(tankManagerObj));

	TankManager::SpawnCounts counts{ tankManager.SpawnTanks() };

	// Teleporter
	auto teleporterObj = std::make_unique<mg::GameObject>("Teleporter");
	teleporterObj->AddComponent<Teleporter>(gridComp, tankManager);
	auto& telepSprite = teleporterObj->AddComponent<mg::Sprite>(Files::EntitySpriteSheet);
	telepSprite.SetTileSize({ 2, 2 });
	telepSprite.SetSprite({ 2, 1 });
	telepSprite.SetPivot(telepSprite.Size() / 4.f);

	
	auto& telepColl = teleporterObj->AddComponent<mg::BoxCollider2D>(); 
	telepColl.SetCenter(telepSprite.Size() / 4.f);
	telepColl.CollisionLayer = static_cast<uint32_t>(GameCollisionLayer::All);
	telepColl.CollisionMask = static_cast<uint32_t>(GameCollisionLayer::Good);
	sceneOut.Add(std::move(teleporterObj));


	// UI
	auto hudManager = std::make_unique<mg::GameObject>("HUD_Manager");
	hudManager->AddComponent<HUDManager>();
	sceneOut.Add(std::move(hudManager));


// Audio
	auto& audioSystem = mg::SoundServiceLocator::Fetch();
	audioSystem.PlayMusic({ Files::GameMusic, "music", -1 });

	ApplyDefault(sceneOut);

	return counts;
}

void SceneLoading::LoadMainMenuScene(mg::Scene& sceneOut)
{
	// Header
	{
		auto obj = std::make_unique<mg::GameObject>("Header_Text", glm::vec2(50.f, 40.f));
		auto& textComp = obj->AddComponent<mg::TextComponent>("joystixmonospace-regular.otf", 32);
		textComp.SetText("TRON BATTLETANKS");
		textComp.SetColor({ 255, 255, 0, 255 });

		sceneOut.Add(std::move(obj));
	}


	 // Current game mode
	{
		auto obj = std::make_unique<mg::GameObject>("Mode_Text", glm::vec2(50.f, 140.f));

		auto& textComp = obj->AddComponent<mg::TextComponent>("joystixmonospace-regular.otf", 20);
		textComp.SetColor({ 255,255,255,255 });

		obj->AddComponent<GameModeUI>();

		sceneOut.Add(std::move(obj));
	}

	// Instructions
	{
		auto obj = std::make_unique<mg::GameObject>("Instruction_Text", glm::vec2(60.f, 220.f));

		auto& textComp = obj->AddComponent<mg::TextComponent>("joystixmonospace-regular.otf", 18);
		textComp.SetText("  CTRL / Gamepad X : Start\n"  "X / Gamepad A : Change Mode"  );
		textComp.SetColor({ 200,200,200,255 });

		sceneOut.Add(std::move(obj));
	}

	// Start game
	{
		auto keyboard = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::KeyboardKey::LCtrl), mg::InputBinding::DeviceType::Keyboard,
			std::make_unique<ContinueSceneCommand>(), mg::InputBinding::TriggerType::Released);

		sceneOut.InputSystem().AddBinding(std::move(keyboard));

		auto gamepad = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::GamepadButton::X), mg::InputBinding::DeviceType::Gamepad,
			std::make_unique<ContinueSceneCommand>(), mg::InputBinding::TriggerType::Released);

		sceneOut.InputSystem().AddBinding(std::move(gamepad));
	}

	// Change game mode
	{
		auto keyboard = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::KeyboardKey::X), mg::InputBinding::DeviceType::Keyboard,
			std::make_unique<ToggleGameModeCommand>(), mg::InputBinding::TriggerType::Released);

		sceneOut.InputSystem().AddBinding(std::move(keyboard));

		auto gamepad = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::GamepadButton::A), mg::InputBinding::DeviceType::Gamepad,
			std::make_unique<ToggleGameModeCommand>(), mg::InputBinding::TriggerType::Released);

		sceneOut.InputSystem().AddBinding(std::move(gamepad));
	}

	// Audio
	auto& audioSystem = mg::SoundServiceLocator::Fetch();
	audioSystem.StopMusic();
	audioSystem.PlaySFX({ Files::StartSFX, "Start_Jingle"});

	ApplyDefault(sceneOut);
}

void SceneLoading::LoadScoreSavingScene(mg::Scene& sceneOut)
{
	auto scoreDispObj = std::make_unique<mg::GameObject>("ScoreDisplay", glm::vec2(20.f, 20.f));
	scoreDispObj->AddComponent<TotalScoreUI>();
	scoreDispObj->AddComponent<mg::TextComponent>("joystixmonospace-regular.otf", 32);

	auto saverObj = std::make_unique<mg::GameObject>("ScoreSaver", glm::vec2(20, 60.f));
	saverObj->AddComponent<ScoreSaver>();
	saverObj->AddComponent<mg::TextComponent>("joystixmonospace-regular.otf", 32);


	   // Save score input
	{
		auto keyboard = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::KeyboardKey::X), mg::InputBinding::DeviceType::Keyboard,
			std::make_unique<SaveScoreCommand>(*saverObj), mg::InputBinding::TriggerType::Released
		);
		sceneOut.InputSystem().AddBinding(std::move(keyboard));

		auto gamepad = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::GamepadButton::A), mg::InputBinding::DeviceType::Gamepad,
			std::make_unique<SaveScoreCommand>(*saverObj), mg::InputBinding::TriggerType::Released
		);
		sceneOut.InputSystem().AddBinding(std::move(gamepad));
	}

	// Cycle up command
	{
		auto keyboard = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::KeyboardKey::Up), mg::InputBinding::DeviceType::Keyboard,
			std::make_unique<ScrollLetterCommand>(*saverObj), mg::InputBinding::TriggerType::Released
		);
		sceneOut.InputSystem().AddBinding(std::move(keyboard));

		auto gamepad = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::GamepadButton::DPadUp), mg::InputBinding::DeviceType::Gamepad,
			std::make_unique<ScrollLetterCommand>(*saverObj), mg::InputBinding::TriggerType::Released
		);
		sceneOut.InputSystem().AddBinding(std::move(gamepad));
	}

	// Cycle down command
	{
		auto keyboard = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::KeyboardKey::Down), mg::InputBinding::DeviceType::Keyboard,
			std::make_unique<ScrollLetterCommand>(*saverObj, true), mg::InputBinding::TriggerType::Released
		);
		sceneOut.InputSystem().AddBinding(std::move(keyboard));

		auto gamepad = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::GamepadButton::DPadDown), mg::InputBinding::DeviceType::Gamepad,
			std::make_unique<ScrollLetterCommand>(*saverObj, true), mg::InputBinding::TriggerType::Released
		);
		sceneOut.InputSystem().AddBinding(std::move(gamepad));
	}

	// Cancel command
	{
		auto keyboard = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::KeyboardKey::Z), mg::InputBinding::DeviceType::Keyboard,
			std::make_unique<CancelCharacter>(*saverObj), mg::InputBinding::TriggerType::Released
		);
		sceneOut.InputSystem().AddBinding(std::move(keyboard));

		auto gamepad = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::GamepadButton::B), mg::InputBinding::DeviceType::Gamepad,
			std::make_unique<CancelCharacter>(*saverObj), mg::InputBinding::TriggerType::Released
		);
		sceneOut.InputSystem().AddBinding(std::move(gamepad));
	}

	sceneOut.Add(std::move(scoreDispObj));
	sceneOut.Add(std::move(saverObj));

	// Audio
	auto& audioSystem = mg::SoundServiceLocator::Fetch();
	audioSystem.StopMusic();
	audioSystem.PlaySFX({ Files::WinSFX, "Win" }); // TODO: Play upon level win instead

	ApplyDefault(sceneOut);
}

void SceneLoading::LoadScoreboardScene(mg::Scene& sceneOut)
{
	auto boardObj = std::make_unique<mg::GameObject>("ScoreBoard", glm::vec2(50.f, 50.f));
	boardObj->AddComponent<mg::TextComponent>("joystixmonospace-regular.otf", 24);
	boardObj->AddComponent<ScoreBoardUI>();

	sceneOut.Add(std::move(boardObj));

			// Continue command
	{
		auto continueSceneCommand = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::KeyboardKey::LCtrl), mg::InputBinding::DeviceType::Keyboard,
			std::make_unique<ContinueSceneCommand>(), mg::InputBinding::TriggerType::Released
		);
		sceneOut.InputSystem().AddBinding(std::move(continueSceneCommand));
	}

	{
		auto continueSceneCommand = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::GamepadButton::X), mg::InputBinding::DeviceType::Gamepad,
			std::make_unique<ContinueSceneCommand>(), mg::InputBinding::TriggerType::Released
		);
		sceneOut.InputSystem().AddBinding(std::move(continueSceneCommand));
	}

	// Audio
	auto& audioSystem = mg::SoundServiceLocator::Fetch();
	audioSystem.StopMusic();
	audioSystem.PlaySFX({ Files::FailureSFX, "Failure" }); // TODO: play upon death instead

	ApplyDefault(sceneOut);
}

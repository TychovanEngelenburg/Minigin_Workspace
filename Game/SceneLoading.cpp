#include "SceneLoading.h"
#include "Minigin/Scene.h"

// GameObject
#include "Minigin/GameObject.h"
#include "EngineComponents/TextComponent.h"
#include "EngineComponents/Sprite.h"
#include "UI/FPS_UI.h"
#include "UI/PlayerHealth_UI.h"
#include "Grid/GameGrid.h"
// Input
#include "Minigin/InputHandling/InputBinding.h"
#include "Minigin/InputHandling/InputCodes.h"
#include "Minigin/InputHandling/InputManager.h"
#include "Commands/MoveTankCommand.h"
#include "Commands/DamageTankCommand.h"
#include "Commands/StartGameCommand.h"
#include "Tank/TankHealth.h"
#include "Tank/TankMovement.h"

void SceneLoading::LoadTestScene(mg::Scene& sceneOut)
{
#pragma region Environment
	auto grid = std::make_unique<mg::GameObject>("Grid", glm::vec3(20, 100.f, 0.f));
	{
		grid->AddComponent<GameGrid>("LevelData/01.lvl", 16.f);
	}

#pragma endregion Environment
	// Player characters initialisation. 
	//auto gamepadPlayer = std::make_unique<mg::GameObject>("Player1", glm::vec3(20, 100.f, 0.f));
	//{
	//	auto& sprite = gamepadPlayer->AddComponent<mg::Sprite>("T_SpriteSheet_Tron.png", mg::SpriteSheet{ 13, 5 });
	//	sprite.SetSprite(10, 0);

	//	gamepadPlayer->AddComponent<TankHealth>();

	//	// TODO: look into moving bindings
	//	auto moveLeft = std::make_unique<mg::InputBinding>(
	//		0, static_cast<int>(mg::Keycodes::GamepadButton::DPadLeft), mg::InputBinding::DeviceType::Gamepad,
	//		std::make_unique<MoveTankCommand>(gamepadPlayer.get(), glm::vec2(-1.f, 0.f), 200.f), mg::InputBinding::TriggerType::Held
	//	);
	//	sceneOut.Input().AddBinding(std::move(moveLeft));

	//	auto moveUp = std::make_unique<mg::InputBinding>(
	//		0, static_cast<int>(mg::Keycodes::GamepadButton::DPadUp), mg::InputBinding::DeviceType::Gamepad,
	//		std::make_unique<MoveTankCommand>(gamepadPlayer.get(), glm::vec2(0.f, -1.f), 200.f), mg::InputBinding::TriggerType::Held
	//	);
	//	sceneOut.Input().AddBinding(std::move(moveUp));

	//	auto moveRight = std::make_unique<mg::InputBinding>(
	//		0, static_cast<int>(mg::Keycodes::GamepadButton::DPadRight), mg::InputBinding::DeviceType::Gamepad,
	//		std::make_unique<MoveTankCommand>(gamepadPlayer.get(), glm::vec2(1.f, 0.f), 200.f), mg::InputBinding::TriggerType::Held
	//	);
	//	sceneOut.Input().AddBinding(std::move(moveRight));

	//	auto moveDown = std::make_unique<mg::InputBinding>(
	//		0, static_cast<int>(mg::Keycodes::GamepadButton::DPadDown), mg::InputBinding::DeviceType::Gamepad,
	//		std::make_unique<MoveTankCommand>(gamepadPlayer.get(), glm::vec2(0.f, 1.f), 200.f), mg::InputBinding::TriggerType::Held
	//	);
	//	sceneOut.Input().AddBinding(std::move(moveDown));

	//	auto doDamage = std::make_unique<mg::InputBinding>(
	//		0, static_cast<int>(mg::Keycodes::GamepadButton::X), mg::InputBinding::DeviceType::Gamepad,
	//		std::make_unique<DamageTankCommand>(gamepadPlayer.get(), 1)
	//	);
	//	sceneOut.Input().AddBinding(std::move(doDamage));
	//}

	auto keyboardPlayer = std::make_unique<mg::GameObject>("Player2", glm::vec3(20, 120.f, 0.f));
	{
		auto& sprite = keyboardPlayer->AddComponent<mg::Sprite>("T_SpriteSheet_Tron.png", mg::SpriteSheet{ 13, 5 });
		sprite.SetSprite(8, 0);

		keyboardPlayer->AddComponent<TankHealth>();
		keyboardPlayer->AddComponent<TankMovement>(grid->GetComponent<GameGrid>(), 100.f);

		// TODO: look into moving bindings
		auto moveLeft = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::KeyboardKey::A), mg::InputBinding::DeviceType::Keyboard,
			std::make_unique<MoveTankCommand>(keyboardPlayer.get(), TankMovement::Direction::Left), mg::InputBinding::TriggerType::Held
		);
		sceneOut.Input().AddBinding(std::move(moveLeft));

		auto moveUp = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::KeyboardKey::W), mg::InputBinding::DeviceType::Keyboard,
			std::make_unique<MoveTankCommand>(keyboardPlayer.get(), TankMovement::Direction::Up), mg::InputBinding::TriggerType::Held
		);
		sceneOut.Input().AddBinding(std::move(moveUp));

		auto moveRight = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::KeyboardKey::D), mg::InputBinding::DeviceType::Keyboard,
			std::make_unique<MoveTankCommand>(keyboardPlayer.get(), TankMovement::Direction::Right), mg::InputBinding::TriggerType::Held
		);
		sceneOut.Input().AddBinding(std::move(moveRight));

		auto moveDown = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::KeyboardKey::S), mg::InputBinding::DeviceType::Keyboard,
			std::make_unique<MoveTankCommand>(keyboardPlayer.get(), TankMovement::Direction::Down), mg::InputBinding::TriggerType::Held
		);
		sceneOut.Input().AddBinding(std::move(moveDown));


		auto doDamage = std::make_unique<mg::InputBinding>(
			0, static_cast<int>(mg::Keycodes::KeyboardKey::C), mg::InputBinding::DeviceType::Keyboard,
			std::make_unique<DamageTankCommand>(keyboardPlayer.get(), 1)
		);
		sceneOut.Input().AddBinding(std::move(doDamage));
	}



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

		auto& healthDisp = playerTwoHealthUI->AddComponent<PlayerHealth_UI>();
		keyboardPlayer->GetComponent<TankHealth>()->AddListener(&healthDisp);
	}

	yPos += textSize + marginSize;
	// Score element here
#pragma endregion UI

	// TODO: Make gameobjects own their children,
	// add a Z-index (decide which child is drawn before which).
	// in general transform needs a complete rework. (which is why I have avoided parenting for now)

	sceneOut.Add(std::move(grid));

	//sceneOut.Add(std::move(gamepadPlayer));
	sceneOut.Add(std::move(keyboardPlayer));

	sceneOut.Add(std::move(fpsCounterUI));
	sceneOut.Add(std::move(playerOneInfoUI));
	sceneOut.Add(std::move(playerTwoInfoUI));
	sceneOut.Add(std::move(playerOneHealthUI));
	sceneOut.Add(std::move(playerTwoHealthUI));

	sceneOut.Awake();

	auto& audioSystem = mg::SoundServiceLocator::Fetch();
	audioSystem.PlayMusic({ "./Data/Audio_Tron1982/03_IO_Tower.wav", "music", -1 });
	audioSystem.SetMusicVolume(.5f);
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
	sceneOut.Input().AddBinding(std::move(startGameInput));

	startGameInput = std::make_unique<mg::InputBinding>(
		0, static_cast<int>(mg::Keycodes::GamepadButton::A), mg::InputBinding::DeviceType::Gamepad,
		std::make_unique<StartGameCommand>(), mg::InputBinding::TriggerType::Released
	);
	sceneOut.Input().AddBinding(std::move(startGameInput));
}

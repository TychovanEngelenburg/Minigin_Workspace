#include <SDL3/SDL_main.h>
#include <SDL3/SDL_main_impl.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include <glm/fwd.hpp>
#include <filesystem>
#include <utility>
#include <memory>

#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)
// Used for exception-handling
#include <iostream>
#endif

// Game
#include "Minigin.h"
#include "SceneManager.h"
#include "Scene.h"

// GameObject
#include "GameObject.h"
#include "EngineComponents/TextComponent.h"
#include "EngineComponents/Sprite.h"
#include "FPS_Display.h"
#include "PlayerMovementComp.h"

// Input
#include "InputHandling/InputBinding.h"
#include "InputHandling/InputCodes.h"
#include "InputHandling/InputManager.h"
#include "MovePlayerCommand.h"

/// <summary>
/// This script and the surrounding "Game" folder is a temporary stand in for the eventual external game project. 
/// It's purpose is to test engine functions and it should not be included in the final engine library!
/// </summary>

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto object = std::make_unique<dae::GameObject>("Background");
	{
		object->AddComponent<dae::Sprite>("background.png");
		scene.Add(std::move(object));
	}


	object = std::make_unique<dae::GameObject>("Logo", glm::vec3(358.f, 180.f, 0.f));
	{
		object->AddComponent<dae::Sprite>("logo.png");
		scene.Add(std::move(object));
	}

	object = std::make_unique<dae::GameObject>("Header_Text", glm::vec3(292.f, 20.f, 0.f));
	{
		auto& textComp = object->AddComponent<dae::TextComponent>("Programming 4 Assignment", "Lingua.otf", 36);
		textComp.SetColor({ 255, 255, 0, 255 });
		scene.Add(std::move(object));
	}

	object = std::make_unique<dae::GameObject>("FPS_Counter");
	{
		auto& textComp = object->AddComponent<dae::TextComponent>("00", "Lingua.otf", 36);
		textComp.SetColor({ 255, 255, 0, 255 });
		object->AddComponent<dae::FPS_Display>();
		scene.Add(std::move(object));
	}



	// Player characters initialisation. 
	object = std::make_unique<dae::GameObject>("Gamepad1");
	{
		auto& sprite = object->AddComponent<dae::Sprite>("T_SpriteSheet_Tron.png", dae::SpriteSheet(13, 5));
		sprite.SetSprite(10, 0);

		object->AddComponent<PlayerMovementComp>(200.f);

		auto moveLeft = std::make_unique<dae::InputBinding>(
			0, static_cast<int>(dae::Keycodes::GamepadButton::DPadLeft), dae::InputBinding::DeviceType::Gamepad,
			std::make_unique<MovePlayerCommand>(object.get(), glm::vec2(-1.f, 0.f)), dae::InputBinding::TriggerType::Held
		);
		dae::InputManager::GetInstance().AddBinding(std::move(moveLeft));

		auto moveUp = std::make_unique<dae::InputBinding>(
			0, static_cast<int>(dae::Keycodes::GamepadButton::DPadUp), dae::InputBinding::DeviceType::Gamepad,
			std::make_unique<MovePlayerCommand>(object.get(), glm::vec2(0.f, -1.f)), dae::InputBinding::TriggerType::Held
		);
		dae::InputManager::GetInstance().AddBinding(std::move(moveUp));

		auto moveRight = std::make_unique<dae::InputBinding>(
			0, static_cast<int>(dae::Keycodes::GamepadButton::DPadRight), dae::InputBinding::DeviceType::Gamepad,
			std::make_unique<MovePlayerCommand>(object.get(), glm::vec2(1.f, 0.f)), dae::InputBinding::TriggerType::Held
		);
		dae::InputManager::GetInstance().AddBinding(std::move(moveRight));

		auto moveDown = std::make_unique<dae::InputBinding>(
			0, static_cast<int>(dae::Keycodes::GamepadButton::DPadDown), dae::InputBinding::DeviceType::Gamepad,
			std::make_unique<MovePlayerCommand>(object.get(), glm::vec2(0.f, 1.f)), dae::InputBinding::TriggerType::Held
		);
		dae::InputManager::GetInstance().AddBinding(std::move(moveDown));
		scene.Add(std::move(object));
	}

	object = std::make_unique<dae::GameObject>("Keyboard");
	{
		auto& sprite = object->AddComponent<dae::Sprite>("T_SpriteSheet_Tron.png", dae::SpriteSheet(13, 5));
		sprite.SetSprite(8, 0);

		object->AddComponent<PlayerMovementComp>(100.f);

		auto moveLeft = std::make_unique<dae::InputBinding>(
			0, static_cast<int>(dae::Keycodes::KeyboardKey::A), dae::InputBinding::DeviceType::Keyboard,
			std::make_unique<MovePlayerCommand>(object.get(), glm::vec2(-1.f, 0.f)), dae::InputBinding::TriggerType::Held
		);
		dae::InputManager::GetInstance().AddBinding(std::move(moveLeft));

		auto moveUp = std::make_unique<dae::InputBinding>(
			0, static_cast<int>(dae::Keycodes::KeyboardKey::W), dae::InputBinding::DeviceType::Keyboard,
			std::make_unique<MovePlayerCommand>(object.get(), glm::vec2(0.f, -1.f)), dae::InputBinding::TriggerType::Held
		);
		dae::InputManager::GetInstance().AddBinding(std::move(moveUp));

		auto moveRight = std::make_unique<dae::InputBinding>(
			0, static_cast<int>(dae::Keycodes::KeyboardKey::D), dae::InputBinding::DeviceType::Keyboard,
			std::make_unique<MovePlayerCommand>(object.get(), glm::vec2(1.f, 0.f)), dae::InputBinding::TriggerType::Held
		);
		dae::InputManager::GetInstance().AddBinding(std::move(moveRight));

		auto moveDown = std::make_unique<dae::InputBinding>(
			0, static_cast<int>(dae::Keycodes::KeyboardKey::S), dae::InputBinding::DeviceType::Keyboard,
			std::make_unique<MovePlayerCommand>(object.get(), glm::vec2(0.f, 1.f)), dae::InputBinding::TriggerType::Held
		);
		dae::InputManager::GetInstance().AddBinding(std::move(moveDown));
		scene.Add(std::move(object));
	}
}


int main(int, char* [])
{
#if USE_STEAMWORKS
	try
	{
		if (!SteamAPI_Init())
		{
			throw std::runtime_error(std::string("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)."));
		}
	}  
	// Because of SDL_main being "noexcept" there shouldn't be throwing in main.
	catch ( std::exception const& except) 
	{
		std::cerr << "Fatal Error: " << except.what() << "\n";
		return -1;
	}
#endif

#if USE_STEAMWORKS
	SteamAPI_RunCallbacks();
#endif 

#if __EMSCRIPTEN__
	std::filesystem::path data_location = "";
#else
	std::filesystem::path data_location = "./Data/";
	if (!std::filesystem::exists(data_location))
	{
		data_location = "../Data/";
	}
#endif


	dae::Minigin engine(data_location);
	engine.Run(load);


#if USE_STEAMWORKS
	SteamAPI_Shutdown();
#endif
	return 0;
}

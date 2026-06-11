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
#include <Minigin/Core/Minigin.h>
#include <Minigin/Scene/SceneManager.h>


// Audio
#include <Minigin/Audio/SoundServiceLocator.h>
#include <Minigin/Audio/LoggingSoundSystem.h>
#include <Minigin/Audio/SDLSoundSystem.h>

#include "Game/Core/GameContext.h"
#include "Game/Core/GameStates.h"

/// <summary>
/// This script and the surrounding "Game" folder is a temporary stand in for the eventual external game project. 
/// It's purpose is to test engine functions and it should not be included in the final engine library!
/// </summary>

static void load()
{
#if _DEBUG
	mg::SoundServiceLocator::Register(std::make_unique<mg::LoggingSoundSystem<mg::SDLSoundSystem>>());
#else
	mg::SoundServiceLocator::Register(std::make_unique<mg::SDLSoundSystem>());
#endif // _DEBUG

	GameContext::Instance().Init();
}


int main(int, char* [])
{

#if __EMSCRIPTEN__
	std::filesystem::path data_location{ "" };
#else
	std::filesystem::path data_location{ "./Data/" };
	if (!std::filesystem::exists(data_location))
	{
		data_location = "../Data/";
	}
#endif


	mg::Minigin engine(data_location);
	engine.Run(load);

	return 0;
}

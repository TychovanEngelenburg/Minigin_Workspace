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

	//size_t sceneId{ mg::SceneManager::Instance().CreateScene() };
	//auto& scene{ *mg::SceneManager::Instance().GetScene(sceneId)};
	//audioSystem.PlayMusic({ "./Data/Audio_Tron1982/11 - Sounds (Derezzed).wav", "derezzed", -1});


	GameContext::Instance().SetState(std::make_unique<MainMenuState>());

	//SceneLoading::LoadTestScene(scene);

	//mg::SceneManager::Instance().SetActiveScene(sceneId);
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
	catch (std::exception const& except)
	{
		std::cerr << "Fatal Error: " << except.what() << "\n";
		//return -1;
	}
#endif

#if USE_STEAMWORKS
	SteamAPI_RunCallbacks();
#endif 

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


#if USE_STEAMWORKS
	SteamAPI_Shutdown();
#endif
	return 0;
}

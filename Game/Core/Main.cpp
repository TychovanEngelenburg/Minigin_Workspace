#include <SDL3/SDL_main.h>
#include <SDL3/SDL_main_impl.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include <filesystem>
#include <memory>


// Game
#include "Game/Core/GameContext.h"


#include <Minigin/Core/EngineConfig.h>
#include <Minigin/Core/Minigin.h>

#include <Minigin/Audio/SoundServiceLocator.h>
#include <Minigin/Audio/LoggingSoundSystem.h>
#include <Minigin/Audio/SDLSoundSystem.h>


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
	mg::EngineConfig config{};
	config.window.title = "Tron_1982_Batteltanks";
	config.window.width = 520;
	config.window.height = 570;

#if __EMSCRIPTEN__
	std::filesystem::path data_location{ "" };
#else
	std::filesystem::path data_location{ "./Data/" };
	if (!std::filesystem::exists(data_location))
	{
		data_location = "../Data/";
	}
#endif
	config.assetPath = data_location;


	mg::Minigin engine(config);
	engine.Run(load);

	return 0;
}

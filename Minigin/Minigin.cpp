#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include "Minigin.h"

#include "InputHandling/InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "DeltaClock.h"
#include "Renderer.h"

#include <stdexcept>
#include <sstream>
#include <iostream>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_version.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>



// .h includes
#include <filesystem>
#include <functional>
#include <memory>

SDL_Window* g_window{};

static void LogSDLVersion(std::string const& message, int major, int minor, int patch)
{
#if WIN32
	std::stringstream ss;
	ss << message << major << "." << minor << "." << patch << "\n";
	OutputDebugString(ss.str().c_str());
#else
	std::cout << message << major << "." << minor << "." << patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#include <filesystem>

void LoopCallback(void* arg)
{
	static_cast<mg::Minigin*>(arg)->RunOneFrame();
}
#endif

// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion()
{
	LogSDLVersion("Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
	int version = SDL_GetVersion();
	LogSDLVersion("Linked with SDL ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	// LogSDLVersion("Compiled with SDL_image ",SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION);
	// version = IMG_Version();
	// LogSDLVersion("Linked with SDL_image ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	LogSDLVersion("Compiled with SDL_ttf ", SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_MICRO_VERSION);
	version = TTF_Version();
	LogSDLVersion("Linked with SDL_ttf ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
}

void mg::Minigin::Run(std::function<void()> const& load)
{
	load();
	SceneManager::GetInstance().Start();


#ifndef __EMSCRIPTEN__
	while (!m_quit)
	{
		RunOneFrame();
	}
#else
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif

	SceneManager::GetInstance().End();
}

void mg::Minigin::RunOneFrame()
{

	m_pDeltaClock->Update();

	m_quit = !InputManager::GetInstance().ProcessInput();
	// TODO: Pass input to scene()

	m_lag += DeltaClock::GetDeltaTime();
	while (m_lag >= DeltaClock::GetFixedDeltaTime())
	{
		SceneManager::GetInstance().FixedUpdate();
		m_lag -= DeltaClock::GetFixedDeltaTime();
	}

	SceneManager::GetInstance().Update();

	Renderer::GetInstance().Render();

	SceneManager::GetInstance().LateUpdate();
	ResourceManager::GetInstance().UnloadUnusedResources();
}

mg::Minigin::Minigin(std::filesystem::path const& dataPath)
	: m_pDeltaClock{ std::make_unique<DeltaClock>() }
	, m_lag{}
	, m_quit{ false }
{
	PrintSDLVersion();

	if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
	{
		SDL_Log("Renderer error: %s", SDL_GetError());
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

#if !defined(_WIN32)
	if (!SDL_InitSubSystem(SDL_INIT_GAMEPAD))
	{
		SDL_Log("Renderer error: %s", SDL_GetError());
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
#endif 

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		1024,
		576,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);
	ResourceManager::GetInstance().Init(dataPath);
	InputManager::GetInstance().Init();
}

mg::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

#include "SDL_InputSystem.h"

#include "Minigin/Platform/SDL_GamePad.h"
#include "Minigin/Platform/SDL_Keyboard.h"
#include <SDL3/SDL.h>

mg::SDL_InputSystem::SDL_InputSystem()
{
	SDL_InitSubSystem(SDL_INIT_GAMEPAD);

	m_pKeyboard = std::make_unique<SDL_Keyboard>();

	for (auto& device : m_pGamepads)
	{
		device = nullptr;
	}


	int count = 0;
	SDL_JoystickID* ids = SDL_GetGamepads(&count);

	if (ids)
	{
		int const slots = static_cast<int>(m_pGamepads.size());
		for (int i = 0; i < count && i < slots; ++i)
			m_pGamepads[i] = std::make_unique<SDLGamepad>(ids[i]);

		SDL_free(ids);
	}
}

mg::SDL_InputSystem::~SDL_InputSystem()
{
	for (auto& device : m_pGamepads)
	{
		device.reset();
	}

	SDL_QuitSubSystem(SDL_INIT_GAMEPAD);
}

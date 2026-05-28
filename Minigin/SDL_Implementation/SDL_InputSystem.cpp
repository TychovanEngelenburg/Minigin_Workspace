#include "Minigin/SDL_Implementation/SDL_InputSystem.h"
#include "Minigin/SDL_Implementation/SDL_GamePad.h"
#include "Minigin/SDL_Implementation/SDL_Keyboard.h"

mg::SDL_InputSystem::SDL_InputSystem()
{
	m_pKeyboard = std::make_unique<SDL_Keyboard>();

	for (size_t i = 0; i < m_pGamepads.size(); i++)
	{
		m_pGamepads[i] = std::make_unique<SDLGamepad>(i);
	}
}

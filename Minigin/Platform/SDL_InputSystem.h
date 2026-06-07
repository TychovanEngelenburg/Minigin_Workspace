#ifndef SDL_INPUT_SYSTEM_H
#define SDL_INPUT_SYSTEM_H


#include "Minigin/Input/AbstractInputSystem.h"

namespace mg
{
	class SDL_InputSystem final : public mg::InputSystem 
	{	
	public:
		SDL_InputSystem();
		~SDL_InputSystem();

		SDL_InputSystem(SDL_InputSystem const&) = delete;
		SDL_InputSystem(SDL_InputSystem&&) = delete;
		SDL_InputSystem& operator=(SDL_InputSystem const&) = delete;
		SDL_InputSystem& operator=(SDL_InputSystem&&) = delete;

	};
}

#endif // !SDL_INPUT_SYSTEM_H

#ifndef SDL_INPUT_SYSTEM_H
#define SDL_INPUT_SYSTEM_H


#include "Minigin/InputSystem/AbstractInputSystem.h"

namespace mg
{
	class SDL_InputSystem final : public mg::InputSystem 
	{	
	public:
		SDL_InputSystem();
		~SDL_InputSystem();
	};
}

#endif // !SDL_INPUT_SYSTEM_H

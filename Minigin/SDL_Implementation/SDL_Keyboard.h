#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "Minigin/InputHandling/IInputDevice.h"

#include <memory>

namespace mg
{
	class SDL_Keyboard final : public IInputDevice
	{
	public:
		bool GetButton(int button) const;
		bool GetButtonDown(int button) const;
		bool GetButtonUp(int button) const;

		void Update();

		SDL_Keyboard();

		~SDL_Keyboard();
		SDL_Keyboard(SDL_Keyboard const& other) = delete;
		SDL_Keyboard(SDL_Keyboard&& other) = delete;
		SDL_Keyboard& operator=(SDL_Keyboard const& other) = delete;
		SDL_Keyboard& operator=(SDL_Keyboard&& other) = delete;

	private:
		class KeyboardImpl;
		std::unique_ptr<KeyboardImpl> m_pImpl;
	};
};
#endif // !KEYBOARD_H

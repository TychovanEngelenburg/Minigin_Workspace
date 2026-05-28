#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

//#include "Minigin/Singleton.h"
//#include <memory>
//#include <array>
//
//#include "Minigin/InputHandling/Gamepad.h"
//#include "Minigin/InputHandling/Keyboard.h"


// TODO: Make service locator?

namespace mg
{
	class InputManager final// : public Singleton<InputManager>
	{
	//public:
	//	SDL_Keyboard const* GetKeyboard();
	//	XInput_Gamepad const* GetGamepad(size_t idx);
	//	
	//	void ProcessInput();

	//	void Init();

	//	InputManager() = default;
	//	virtual  ~InputManager() = default;
	//	InputManager(InputManager const& other) = delete;
	//	InputManager(InputManager&& other) = delete;
	//	InputManager& operator=(InputManager const& other) = delete;
	//	InputManager& operator=(InputManager&& other) = delete;

	//private:
	//	friend class Singleton<InputManager>;

	//	std::unique_ptr<SDL_Keyboard> m_pKeyboard{};
	//	std::array<std::unique_ptr<XInput_Gamepad>, 4> m_pGamepads{};

	};
}
#endif // !INPUTMANAGER_H

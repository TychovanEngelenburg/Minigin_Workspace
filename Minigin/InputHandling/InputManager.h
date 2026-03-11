#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Singleton.h"
#include <memory>
#include <vector>
#include <array>
#include "InputHandling/InputBinding.h"

#include "InputHandling/Gamepad.h"
#include "InputHandling/Keyboard.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		void AddBinding(std::unique_ptr<InputBinding> binding);
		void RemoveBinding(InputBinding* binding);

		void Init();

		InputManager() = default;
		virtual  ~InputManager() = default;
		InputManager(InputManager const& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(InputManager const& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;

	private:
		friend class Singleton<InputManager>;

		std::vector<std::unique_ptr<InputBinding>> m_pBindings{};
		std::unique_ptr<Keyboard> m_pKeyboard{};
		std::array<std::unique_ptr<Gamepad>, 4> m_pGamepads{};

	};
}
#endif // !INPUTMANAGER_H

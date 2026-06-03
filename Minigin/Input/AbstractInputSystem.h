#ifndef ABSTRACT_INPUT_SYSTEM_H
#define ABSTRACT_INPUT_SYSTEM_H

#include <memory>
#include <array>
#include "Minigin/Input/IInputDevice.h"


namespace mg
{
	class InputSystem
	{
	public:
		IInputDevice const* GetKeyboard() const;
		IInputDevice const* GetGamepad(size_t idx) const;

		void ProcessInput();

		InputSystem() = default;

		virtual  ~InputSystem() = default;
		InputSystem(InputSystem const& other) = delete;
		InputSystem(InputSystem&& other) = delete;
		InputSystem& operator=(InputSystem const& other) = delete;
		InputSystem& operator=(InputSystem&& other) = delete;

	protected:
		std::unique_ptr<IInputDevice> m_pKeyboard{};
		std::array<std::unique_ptr<IInputDevice>, 4> m_pGamepads{};
	};
}

#endif // !ABSTRACT_INPUT_SYSTEM_H

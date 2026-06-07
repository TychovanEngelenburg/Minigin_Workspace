#ifndef GAMEPAD_H
#define GAMEPAD_H
#include "Minigin/Input/IInputDevice.h"

#include <memory>

namespace mg
{
	class XInput_Gamepad final : public IInputDevice
	{
	public:
		bool GetButton(int button) const override;
		bool GetButtonUp(int button) const override;
		bool GetButtonDown(int button) const override;
		// TODO: GetAxis() polling implementation.

		void Update();

		explicit XInput_Gamepad(int index);

		~XInput_Gamepad();
		XInput_Gamepad(XInput_Gamepad const& other) = delete;
		XInput_Gamepad(XInput_Gamepad&& other) = delete;
		XInput_Gamepad& operator=(XInput_Gamepad const& other) = delete;
		XInput_Gamepad& operator=(XInput_Gamepad&& other) = delete;
	private:
		class GamepadImpl;
		std::unique_ptr<GamepadImpl> m_pImpl;
	};
};
#endif // !GAMEPAD_H

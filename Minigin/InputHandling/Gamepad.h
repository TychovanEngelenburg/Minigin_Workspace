#ifndef GAMEPAD_H
#define GAMEPAD_H
#include "InputHandling/InputDevice.h"
#include <memory>

namespace dae
{
	class Gamepad final : public InputDevice
	{
	public:
		bool GetButton(int button) const override;
		bool GetButtonUp(int button) const override;
		bool GetButtonDown(int button) const override;
		// TODO: GetAxis() polling implementation.

		void Update();

		explicit Gamepad(int index);

		~Gamepad();
		Gamepad(Gamepad const& other) = delete;
		Gamepad(Gamepad&& other) = delete;
		Gamepad& operator=(Gamepad const& other) = delete;
		Gamepad& operator=(Gamepad&& other) = delete;
	private:
		class GamepadImpl;
		std::unique_ptr<GamepadImpl> m_pImpl;

		int m_deviceIndex;
	};
};
#endif // !GAMEPAD_H

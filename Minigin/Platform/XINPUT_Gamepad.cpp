#include "XINPUT_Gamepad.h"

#include "Minigin/Input/InputCodes.h"

#include "Windows.h"
#include <Xinput.h>

class mg::XInput_Gamepad::GamepadImpl
{
public:
	bool GetButton(mg::Keycodes::GamepadButton button) const
	{
		return m_currentState.Gamepad.wButtons & ButtonToXInput(button);
	}

	bool GetButtonUp(mg::Keycodes::GamepadButton button) const
	{
		return m_buttonsReleasedThisFrame & ButtonToXInput(button);

	}

	bool GetButtonDown(mg::Keycodes::GamepadButton button) const
	{
		return m_buttonsPressedThisFrame & ButtonToXInput(button);
	}

	void Update()
	{
		CopyMemory(&m_previousState, &m_currentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));
		XInputGetState(m_deviceIndex, &m_currentState);


		auto buttonChanges = m_currentState.Gamepad.wButtons ^ m_previousState.Gamepad.wButtons;

		m_buttonsPressedThisFrame = buttonChanges & m_currentState.Gamepad.wButtons;
		m_buttonsReleasedThisFrame = buttonChanges & (~m_currentState.Gamepad.wButtons);
	}

	GamepadImpl(int index)
		: m_deviceIndex(index)
	{
	}

private:
	int m_deviceIndex;

	XINPUT_STATE m_previousState{};
	XINPUT_STATE m_currentState{};

	WORD m_buttonsPressedThisFrame{};
	WORD m_buttonsReleasedThisFrame{};

	unsigned int ButtonToXInput(mg::Keycodes::GamepadButton button) const
	{
		switch (button)
		{
			case mg::Keycodes::GamepadButton::DPadUp: return XINPUT_GAMEPAD_DPAD_UP;
			case mg::Keycodes::GamepadButton::DPadDown: return XINPUT_GAMEPAD_DPAD_DOWN;
			case mg::Keycodes::GamepadButton::DPadLeft: return XINPUT_GAMEPAD_DPAD_LEFT;
			case mg::Keycodes::GamepadButton::DPadRight: return XINPUT_GAMEPAD_DPAD_RIGHT;

			case mg::Keycodes::GamepadButton::Start: return XINPUT_GAMEPAD_START;
			case mg::Keycodes::GamepadButton::Back: return XINPUT_GAMEPAD_BACK;

			case mg::Keycodes::GamepadButton::LeftStick: return XINPUT_GAMEPAD_LEFT_THUMB;
			case mg::Keycodes::GamepadButton::RightStick: return XINPUT_GAMEPAD_RIGHT_THUMB;

			case mg::Keycodes::GamepadButton::LeftShoulder: return XINPUT_GAMEPAD_LEFT_SHOULDER;
			case mg::Keycodes::GamepadButton::RightShoulder: return XINPUT_GAMEPAD_RIGHT_SHOULDER;

			case mg::Keycodes::GamepadButton::A: return XINPUT_GAMEPAD_A;
			case mg::Keycodes::GamepadButton::B: return XINPUT_GAMEPAD_B;
			case mg::Keycodes::GamepadButton::X: return XINPUT_GAMEPAD_X;
			case mg::Keycodes::GamepadButton::Y: return XINPUT_GAMEPAD_Y;

			default: return 0;
		}
	}
};


// Base class implementation
bool mg::XInput_Gamepad::GetButton(int buttonId) const
{
	return m_pImpl->GetButton(static_cast<mg::Keycodes::GamepadButton>(buttonId));
}

bool mg::XInput_Gamepad::GetButtonDown(int buttonId) const
{
	return m_pImpl->GetButtonDown(static_cast<mg::Keycodes::GamepadButton>(buttonId));
}

bool mg::XInput_Gamepad::GetButtonUp(int buttonId) const
{
	return m_pImpl->GetButtonUp(static_cast<mg::Keycodes::GamepadButton>(buttonId));
}

void mg::XInput_Gamepad::Update()
{
	m_pImpl->Update();
}

mg::XInput_Gamepad::XInput_Gamepad(int index)
	: m_pImpl(std::make_unique<GamepadImpl>(index))
{
}

mg::XInput_Gamepad::~XInput_Gamepad() = default;

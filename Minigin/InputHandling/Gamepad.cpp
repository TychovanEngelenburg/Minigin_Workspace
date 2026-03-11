#include "Gamepad.h"
#include "InputHandling/InputCodes.h"

#if defined(_WIN32)
#include "Windows.h"
#include <Xinput.h>
#elif(__EMSCRIPTEN__)
#include <SDL3/SDL.h>
#include <array>
#endif

#if defined(_WIN32) // XInput Implementation
class dae::Gamepad::GamepadImpl
{
public:
	bool GetButton(dae::Keycodes::GamepadButton button) const;
	bool GetButtonUp(dae::Keycodes::GamepadButton button) const;
	bool GetButtonDown(dae::Keycodes::GamepadButton button) const;

	void Update(int deviceIdx);

	GamepadImpl();
private:
	XINPUT_STATE m_previousState;
	XINPUT_STATE m_currentState;

	WORD m_buttonsPressedThisFrame;
	WORD m_buttonsReleasedThisFrame;

	unsigned int ButtonToXInput(dae::Keycodes::GamepadButton button) const;
};

bool dae::Gamepad::GamepadImpl::GetButton(dae::Keycodes::GamepadButton button) const
{
	return m_currentState.Gamepad.wButtons & ButtonToXInput(button);
}

bool dae::Gamepad::GamepadImpl::GetButtonUp(dae::Keycodes::GamepadButton button) const
{
	return m_buttonsReleasedThisFrame & ButtonToXInput(button);

}

bool dae::Gamepad::GamepadImpl::GetButtonDown(dae::Keycodes::GamepadButton button) const
{
	return m_buttonsPressedThisFrame & ButtonToXInput(button);
}

void dae::Gamepad::GamepadImpl::Update(int deviceIdx)
{
	CopyMemory(&m_previousState, &m_currentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));

	XInputGetState(deviceIdx, &m_currentState);

	auto buttonChanges = m_currentState.Gamepad.wButtons ^ m_previousState.Gamepad.wButtons;

	m_buttonsPressedThisFrame = buttonChanges & m_currentState.Gamepad.wButtons;
	m_buttonsReleasedThisFrame = buttonChanges & (~m_currentState.Gamepad.wButtons);
}

dae::Gamepad::GamepadImpl::GamepadImpl()
	: m_previousState{}
	, m_currentState{}
	, m_buttonsPressedThisFrame{}
	, m_buttonsReleasedThisFrame{}
{
}

unsigned int dae::Gamepad::GamepadImpl::ButtonToXInput(dae::Keycodes::GamepadButton button) const
{
	switch (button)
	{
		case dae::Keycodes::GamepadButton::DPadUp: return XINPUT_GAMEPAD_DPAD_UP;
		case dae::Keycodes::GamepadButton::DPadDown: return XINPUT_GAMEPAD_DPAD_DOWN;
		case dae::Keycodes::GamepadButton::DPadLeft: return XINPUT_GAMEPAD_DPAD_LEFT;
		case dae::Keycodes::GamepadButton::DPadRight: return XINPUT_GAMEPAD_DPAD_RIGHT;

		case dae::Keycodes::GamepadButton::Start: return XINPUT_GAMEPAD_START;
		case dae::Keycodes::GamepadButton::Back: return XINPUT_GAMEPAD_BACK;

		case dae::Keycodes::GamepadButton::LeftStick: return XINPUT_GAMEPAD_LEFT_THUMB;
		case dae::Keycodes::GamepadButton::RightStick: return XINPUT_GAMEPAD_RIGHT_THUMB;

		case dae::Keycodes::GamepadButton::LeftShoulder: return XINPUT_GAMEPAD_LEFT_SHOULDER;
		case dae::Keycodes::GamepadButton::RightShoulder: return XINPUT_GAMEPAD_RIGHT_SHOULDER;

		case dae::Keycodes::GamepadButton::A: return XINPUT_GAMEPAD_A;
		case dae::Keycodes::GamepadButton::B: return XINPUT_GAMEPAD_B;
		case dae::Keycodes::GamepadButton::X: return XINPUT_GAMEPAD_X;
		case dae::Keycodes::GamepadButton::Y: return XINPUT_GAMEPAD_Y;

		default: return 0;
	}
}

#elif defined(__EMSCRIPTEN__) // SDL Implementation
class dae::Gamepad::GamepadImpl
{
public:
	bool GetButton(dae::Keycodes::GamepadButton button) const;
	bool GetButtonDown(dae::Keycodes::GamepadButton button) const;
	bool GetButtonUp(dae::Keycodes::GamepadButton button) const;

	void Update(int);

	GamepadImpl();

private:
	SDL_Gamepad* m_gamepad{};

	std::array<bool, static_cast<int>(Keycodes::GamepadButton::ButtonCount)> m_current;
	std::array<bool, static_cast<int>(Keycodes::GamepadButton::ButtonCount)> m_previous;

	SDL_GamepadButton ButtonToSDL(Keycodes::GamepadButton button) const;
};

bool dae::Gamepad::GamepadImpl::GetButton(dae::Keycodes::GamepadButton button) const
{
	return m_current[static_cast<int>(button)];
}

bool dae::Gamepad::GamepadImpl::GetButtonDown(dae::Keycodes::GamepadButton button) const
{
	return m_current[static_cast<int>(button)] && !m_previous[static_cast<int>(button)];
}

bool dae::Gamepad::GamepadImpl::GetButtonUp(dae::Keycodes::GamepadButton button) const
{
	return !m_current[static_cast<int>(button)] && m_previous[static_cast<int>(button)];
}

dae::Gamepad::GamepadImpl::GamepadImpl()
	: m_current{}
	, m_previous{}
{
}

void dae::Gamepad::GamepadImpl::Update(int)
{
	SDL_PumpEvents();

	m_previous = m_current;

	if (!m_gamepad)
	{
		if (!SDL_IsGamepad(0))
		{
			return;
		}

		m_gamepad = SDL_OpenGamepad(0);
	}

	for (size_t buttonIdx{}; buttonIdx < m_current.size(); ++buttonIdx)
	{
		auto sdlButton = ButtonToSDL(static_cast<Keycodes::GamepadButton>(buttonIdx));

		if (sdlButton != SDL_GAMEPAD_BUTTON_INVALID)
		{
			m_current[buttonIdx] = SDL_GetGamepadButton(m_gamepad, sdlButton);
		}
	}
}

SDL_GamepadButton dae::Gamepad::GamepadImpl::ButtonToSDL(Keycodes::GamepadButton button) const
{
	switch (button)
	{
		case Keycodes::GamepadButton::A: return SDL_GAMEPAD_BUTTON_SOUTH;
		case Keycodes::GamepadButton::B: return SDL_GAMEPAD_BUTTON_EAST;
		case Keycodes::GamepadButton::X: return SDL_GAMEPAD_BUTTON_WEST;
		case Keycodes::GamepadButton::Y: return SDL_GAMEPAD_BUTTON_NORTH;

		case Keycodes::GamepadButton::DPadUp: return SDL_GAMEPAD_BUTTON_DPAD_UP;
		case Keycodes::GamepadButton::DPadDown: return SDL_GAMEPAD_BUTTON_DPAD_DOWN;
		case Keycodes::GamepadButton::DPadLeft: return SDL_GAMEPAD_BUTTON_DPAD_LEFT;
		case Keycodes::GamepadButton::DPadRight: return SDL_GAMEPAD_BUTTON_DPAD_RIGHT;

		case Keycodes::GamepadButton::Start: return SDL_GAMEPAD_BUTTON_START;
		case Keycodes::GamepadButton::Back: return SDL_GAMEPAD_BUTTON_BACK;

		case Keycodes::GamepadButton::LeftShoulder: return SDL_GAMEPAD_BUTTON_LEFT_SHOULDER;
		case Keycodes::GamepadButton::RightShoulder: return SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER;

		case Keycodes::GamepadButton::LeftStick: return SDL_GAMEPAD_BUTTON_LEFT_STICK;
		case Keycodes::GamepadButton::RightStick: return SDL_GAMEPAD_BUTTON_RIGHT_STICK;

		default: return SDL_GAMEPAD_BUTTON_INVALID;
	}
}
#endif


// Base class implementation
bool dae::Gamepad::GetButton(int buttonId) const
{
	return m_pImpl->GetButton(static_cast<dae::Keycodes::GamepadButton>(buttonId));
}

bool dae::Gamepad::GetButtonDown(int buttonId) const
{
	return m_pImpl->GetButtonDown(static_cast<dae::Keycodes::GamepadButton>(buttonId));
}

bool dae::Gamepad::GetButtonUp(int buttonId) const
{
	return m_pImpl->GetButtonUp(static_cast<dae::Keycodes::GamepadButton>(buttonId));
}

void dae::Gamepad::Update()
{
	m_pImpl->Update(m_deviceIndex);
}

dae::Gamepad::Gamepad(int index)
	: m_pImpl{ std::make_unique<GamepadImpl>() }
	, m_deviceIndex{ index }
{
}

dae::Gamepad::~Gamepad() = default;

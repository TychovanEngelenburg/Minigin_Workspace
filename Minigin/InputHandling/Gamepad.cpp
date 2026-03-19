#include "Gamepad.h"
#include "InputHandling/InputCodes.h"

#if defined(_WIN32)
#include "Windows.h"
#include <Xinput.h>
#else
#include <SDL3/SDL.h>
#include <array>
#endif

#if defined(_WIN32) // XInput Implementation
class mg::Gamepad::GamepadImpl
{
public:
	bool GetButton(mg::Keycodes::GamepadButton button) const;
	bool GetButtonUp(mg::Keycodes::GamepadButton button) const;
	bool GetButtonDown(mg::Keycodes::GamepadButton button) const;

	void Update();

	GamepadImpl(int index);
private:
	int m_deviceIndex;

	XINPUT_STATE m_previousState;
	XINPUT_STATE m_currentState;

	WORD m_buttonsPressedThisFrame;
	WORD m_buttonsReleasedThisFrame;

	unsigned int ButtonToXInput(mg::Keycodes::GamepadButton button) const;
};

bool mg::Gamepad::GamepadImpl::GetButton(mg::Keycodes::GamepadButton button) const
{
	return m_currentState.Gamepad.wButtons & ButtonToXInput(button);
}

bool mg::Gamepad::GamepadImpl::GetButtonUp(mg::Keycodes::GamepadButton button) const
{
	return m_buttonsReleasedThisFrame & ButtonToXInput(button);

}

bool mg::Gamepad::GamepadImpl::GetButtonDown(mg::Keycodes::GamepadButton button) const
{
	return m_buttonsPressedThisFrame & ButtonToXInput(button);
}

void mg::Gamepad::GamepadImpl::Update()
{
	CopyMemory(&m_previousState, &m_currentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));
	XInputGetState(m_deviceIndex, &m_currentState);


	auto buttonChanges = m_currentState.Gamepad.wButtons ^ m_previousState.Gamepad.wButtons;

	m_buttonsPressedThisFrame = buttonChanges & m_currentState.Gamepad.wButtons;
	m_buttonsReleasedThisFrame = buttonChanges & (~m_currentState.Gamepad.wButtons);
}

mg::Gamepad::GamepadImpl::GamepadImpl(int index)
	: m_deviceIndex{ index }
	, m_previousState{}
	, m_currentState{}
	, m_buttonsPressedThisFrame{}
	, m_buttonsReleasedThisFrame{}
{
}

unsigned int mg::Gamepad::GamepadImpl::ButtonToXInput(mg::Keycodes::GamepadButton button) const
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

#else // SDL Implementation
class mg::Gamepad::GamepadImpl
{
public:
	bool GetButton(mg::Keycodes::GamepadButton button) const;
	bool GetButtonDown(mg::Keycodes::GamepadButton button) const;
	bool GetButtonUp(mg::Keycodes::GamepadButton button) const;

	void Update();

	GamepadImpl(int index);
	~GamepadImpl();
	GamepadImpl(GamepadImpl const& other) = delete;
	GamepadImpl(GamepadImpl&& other) = delete;
	GamepadImpl& operator=(GamepadImpl const& other) = delete;
	GamepadImpl& operator=(GamepadImpl&& other) = delete;

private:
	SDL_Gamepad* m_gamepad{};
	int m_deviceIndex;

	std::array<bool, static_cast<int>(Keycodes::GamepadButton::ButtonCount)> m_current;
	std::array<bool, static_cast<int>(Keycodes::GamepadButton::ButtonCount)> m_previous;

	SDL_GamepadButton ButtonToSDL(Keycodes::GamepadButton button) const;
};

bool mg::Gamepad::GamepadImpl::GetButton(mg::Keycodes::GamepadButton button) const
{
	return m_current[static_cast<int>(button)];
}

bool mg::Gamepad::GamepadImpl::GetButtonDown(mg::Keycodes::GamepadButton button) const
{
	return m_current[static_cast<int>(button)] && !m_previous[static_cast<int>(button)];
}

bool mg::Gamepad::GamepadImpl::GetButtonUp(mg::Keycodes::GamepadButton button) const
{
	return !m_current[static_cast<int>(button)] && m_previous[static_cast<int>(button)];
}

mg::Gamepad::GamepadImpl::GamepadImpl(int index)
	: m_gamepad{ nullptr }
	, m_deviceIndex{ index }
	, m_current{}
	, m_previous{}
{
}

mg::Gamepad::GamepadImpl::~GamepadImpl()
{
	if (m_gamepad)
	{
		SDL_CloseGamepad(m_gamepad);
	}
}


void mg::Gamepad::GamepadImpl::Update()
{
	SDL_PumpEvents();
	m_previous = m_current;

	if (!m_gamepad)
	{
		if (!SDL_IsGamepad(m_deviceIndex))
		{
			return;
		}

		m_gamepad = SDL_OpenGamepad(m_deviceIndex);

		if (!m_gamepad)
		{
			return;
		}
	}

	for (size_t buttonIdx{}; buttonIdx < m_current.size(); ++buttonIdx)
	{
		auto sdlButton = ButtonToSDL(static_cast<Keycodes::GamepadButton>(buttonIdx));

		if (sdlButton != SDL_GAMEPAD_BUTTON_INVALID)
		{
			m_current[buttonIdx] = SDL_GetGamepadButton(m_gamepad, sdlButton);
		}
		else
		{
			m_current[buttonIdx] = false;
		}
	}
}

SDL_GamepadButton mg::Gamepad::GamepadImpl::ButtonToSDL(Keycodes::GamepadButton button) const
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
bool mg::Gamepad::GetButton(int buttonId) const
{
	return m_pImpl->GetButton(static_cast<mg::Keycodes::GamepadButton>(buttonId));
}

bool mg::Gamepad::GetButtonDown(int buttonId) const
{
	return m_pImpl->GetButtonDown(static_cast<mg::Keycodes::GamepadButton>(buttonId));
}

bool mg::Gamepad::GetButtonUp(int buttonId) const
{
	return m_pImpl->GetButtonUp(static_cast<mg::Keycodes::GamepadButton>(buttonId));
}

void mg::Gamepad::Update()
{
	m_pImpl->Update();
}

mg::Gamepad::Gamepad(int index)
	: m_pImpl{ std::make_unique<GamepadImpl>(index) }
{
}

mg::Gamepad::~Gamepad() = default;

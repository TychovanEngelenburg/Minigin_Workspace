#include "SDL_GamePad.h"

#include "Minigin/Input/InputCodes.h"

#include <SDL3/SDL.h>
#include <array>

class mg::SDLGamepad::GamepadImpl
{
public:
	bool Connected() const
	{
		return m_gamepad != nullptr;
	}

	bool GetButton(mg::Keycodes::GamepadButton button) const
	{
		return m_current[static_cast<int>(button)];
	}

	bool GetButtonDown(mg::Keycodes::GamepadButton button) const
	{
		return m_current[static_cast<int>(button)] && !m_previous[static_cast<int>(button)];
	}

	bool GetButtonUp(mg::Keycodes::GamepadButton button) const
	{
		return !m_current[static_cast<int>(button)] && m_previous[static_cast<int>(button)];
	}

	void Update()
	{
		//SDL_UpdateGamepads();

		m_previous = m_current;

		if (!m_gamepad)
		{
			m_gamepad = SDL_OpenGamepad(m_deviceIndex);

			if (!m_gamepad)
			{
				m_current.fill(false);
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

	explicit GamepadImpl(SDL_JoystickID index)
	: m_deviceIndex( index )
	{
		m_gamepad = SDL_OpenGamepad(index);
	}

	~GamepadImpl()
	{
		if (m_gamepad)
		{
			SDL_CloseGamepad(m_gamepad);
		}
	}

	GamepadImpl(GamepadImpl const& other) = delete;
	GamepadImpl(GamepadImpl&& other) = delete;
	GamepadImpl& operator=(GamepadImpl const& other) = delete;
	GamepadImpl& operator=(GamepadImpl&& other) = delete;

private:
	SDL_Gamepad* m_gamepad{};
	SDL_JoystickID m_deviceIndex{};

	std::array<bool, static_cast<int>(Keycodes::GamepadButton::ButtonCount)> m_current{};
	std::array<bool, static_cast<int>(Keycodes::GamepadButton::ButtonCount)> m_previous{};

	SDL_GamepadButton ButtonToSDL(Keycodes::GamepadButton button) const
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
};


// Base class implementation
bool mg::SDLGamepad::Connected() const
{
	return m_pImpl->Connected();
}

bool mg::SDLGamepad::GetButton(int buttonId) const
{
	return m_pImpl->GetButton(static_cast<mg::Keycodes::GamepadButton>(buttonId));
}

bool mg::SDLGamepad::GetButtonDown(int buttonId) const
{
	return m_pImpl->GetButtonDown(static_cast<mg::Keycodes::GamepadButton>(buttonId));
}

bool mg::SDLGamepad::GetButtonUp(int buttonId) const
{
	return m_pImpl->GetButtonUp(static_cast<mg::Keycodes::GamepadButton>(buttonId));
}

void mg::SDLGamepad::Update()
{
	m_pImpl->Update();
}

mg::SDLGamepad::SDLGamepad(int index)
	: m_pImpl( std::make_unique<GamepadImpl>(index) )
{
}

mg::SDLGamepad::~SDLGamepad() = default;

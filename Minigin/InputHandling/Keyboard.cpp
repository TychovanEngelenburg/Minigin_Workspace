#include "InputHandling/Keyboard.h"

#include "InputHandling/InputCodes.h"
#include <SDL3/SDL.h>
#include <array>

// SDL Implementation
class dae::Keyboard::KeyboardImpl
{
public:
	bool GetButton(int button) const;
	bool GetButtonDown(int button) const;
	bool GetButtonUp(int button) const;

	void Update();

	KeyboardImpl();

private:
	std::array<bool, static_cast<int>(Keycodes::KeyboardKey::KeyCount)> m_current;
	std::array<bool, static_cast<int>(Keycodes::KeyboardKey::KeyCount)> m_previous;

	SDL_Scancode KeyToSDL(Keycodes::KeyboardKey button) const;
};

bool  dae::Keyboard::KeyboardImpl::GetButton(int button) const
{
	return m_current[button];
}

bool  dae::Keyboard::KeyboardImpl::GetButtonDown(int button) const
{
	return m_current[button] && !m_previous[button];
}

bool  dae::Keyboard::KeyboardImpl::GetButtonUp(int button) const
{
	return !m_current[button] && m_previous[button];
}

void dae::Keyboard::KeyboardImpl::Update()
{
	SDL_PumpEvents();

	m_previous = m_current;

	const bool* sdlState = SDL_GetKeyboardState(nullptr);
	for (size_t keyIdx{}; keyIdx < m_current.size(); ++keyIdx)
	{
		auto stateIdxKey = KeyToSDL(static_cast<dae::Keycodes::KeyboardKey>(keyIdx));
		if (stateIdxKey != SDL_SCANCODE_UNKNOWN)
		{
			if (m_current[keyIdx] != sdlState[stateIdxKey])
			{
				m_current[keyIdx] = sdlState[stateIdxKey];
			}
			//m_current[keyIdx] = sdlState[stateIdxKey];
		}
	}
}

dae::Keyboard::KeyboardImpl::KeyboardImpl()
	: m_current{}
	, m_previous{}
{
}

SDL_Scancode dae::Keyboard::KeyboardImpl::KeyToSDL(dae::Keycodes::KeyboardKey key) const
{
	switch (key)
	{
		case dae::Keycodes::KeyboardKey::A: return SDL_SCANCODE_A;
		case dae::Keycodes::KeyboardKey::B: return SDL_SCANCODE_B;
		case dae::Keycodes::KeyboardKey::C: return SDL_SCANCODE_C;
		case dae::Keycodes::KeyboardKey::D: return SDL_SCANCODE_D;
		case dae::Keycodes::KeyboardKey::E: return SDL_SCANCODE_E;
		case dae::Keycodes::KeyboardKey::F: return SDL_SCANCODE_F;
		case dae::Keycodes::KeyboardKey::G: return SDL_SCANCODE_G;
		case dae::Keycodes::KeyboardKey::H: return SDL_SCANCODE_H;
		case dae::Keycodes::KeyboardKey::I: return SDL_SCANCODE_I;
		case dae::Keycodes::KeyboardKey::J: return SDL_SCANCODE_J;
		case dae::Keycodes::KeyboardKey::K: return SDL_SCANCODE_K;
		case dae::Keycodes::KeyboardKey::L: return SDL_SCANCODE_L;
		case dae::Keycodes::KeyboardKey::M: return SDL_SCANCODE_M;
		case dae::Keycodes::KeyboardKey::N: return SDL_SCANCODE_N;
		case dae::Keycodes::KeyboardKey::O: return SDL_SCANCODE_O;
		case dae::Keycodes::KeyboardKey::P: return SDL_SCANCODE_P;
		case dae::Keycodes::KeyboardKey::Q: return SDL_SCANCODE_Q;
		case dae::Keycodes::KeyboardKey::R: return SDL_SCANCODE_R;
		case dae::Keycodes::KeyboardKey::S: return SDL_SCANCODE_S;
		case dae::Keycodes::KeyboardKey::T: return SDL_SCANCODE_T;
		case dae::Keycodes::KeyboardKey::U: return SDL_SCANCODE_U;
		case dae::Keycodes::KeyboardKey::V: return SDL_SCANCODE_V;
		case dae::Keycodes::KeyboardKey::W: return SDL_SCANCODE_W;
		case dae::Keycodes::KeyboardKey::X: return SDL_SCANCODE_X;
		case dae::Keycodes::KeyboardKey::Y: return SDL_SCANCODE_Y;
		case dae::Keycodes::KeyboardKey::Z: return SDL_SCANCODE_Z;

		case dae::Keycodes::KeyboardKey::Num0: return SDL_SCANCODE_0;
		case dae::Keycodes::KeyboardKey::Num1: return SDL_SCANCODE_1;
		case dae::Keycodes::KeyboardKey::Num2: return SDL_SCANCODE_2;
		case dae::Keycodes::KeyboardKey::Num3: return SDL_SCANCODE_3;
		case dae::Keycodes::KeyboardKey::Num4: return SDL_SCANCODE_4;
		case dae::Keycodes::KeyboardKey::Num5: return SDL_SCANCODE_5;
		case dae::Keycodes::KeyboardKey::Num6: return SDL_SCANCODE_6;
		case dae::Keycodes::KeyboardKey::Num7: return SDL_SCANCODE_7;
		case dae::Keycodes::KeyboardKey::Num8: return SDL_SCANCODE_8;
		case dae::Keycodes::KeyboardKey::Num9: return SDL_SCANCODE_9;

		case dae::Keycodes::KeyboardKey::Space: return SDL_SCANCODE_SPACE;
		case dae::Keycodes::KeyboardKey::Enter: return SDL_SCANCODE_RETURN;
		case dae::Keycodes::KeyboardKey::Escape: return SDL_SCANCODE_ESCAPE;
		case dae::Keycodes::KeyboardKey::Tab: return SDL_SCANCODE_TAB;
		case dae::Keycodes::KeyboardKey::Backspace: return SDL_SCANCODE_BACKSPACE;

		case dae::Keycodes::KeyboardKey::Up: return SDL_SCANCODE_UP;
		case dae::Keycodes::KeyboardKey::Down: return SDL_SCANCODE_DOWN;
		case dae::Keycodes::KeyboardKey::Left: return SDL_SCANCODE_LEFT;
		case dae::Keycodes::KeyboardKey::Right: return SDL_SCANCODE_RIGHT;

		case dae::Keycodes::KeyboardKey::LShift: return SDL_SCANCODE_LSHIFT;
		case dae::Keycodes::KeyboardKey::RShift: return SDL_SCANCODE_RSHIFT;
		case dae::Keycodes::KeyboardKey::LCtrl: return SDL_SCANCODE_LCTRL;
		case dae::Keycodes::KeyboardKey::RCtrl: return SDL_SCANCODE_RCTRL;
		case dae::Keycodes::KeyboardKey::LAlt: return SDL_SCANCODE_LALT;
		case dae::Keycodes::KeyboardKey::RAlt: return SDL_SCANCODE_RALT;

		case dae::Keycodes::KeyboardKey::F1: return SDL_SCANCODE_F1;
		case dae::Keycodes::KeyboardKey::F2: return SDL_SCANCODE_F2;
		case dae::Keycodes::KeyboardKey::F3: return SDL_SCANCODE_F3;
		case dae::Keycodes::KeyboardKey::F4: return SDL_SCANCODE_F4;
		case dae::Keycodes::KeyboardKey::F5: return SDL_SCANCODE_F5;
		case dae::Keycodes::KeyboardKey::F6: return SDL_SCANCODE_F6;
		case dae::Keycodes::KeyboardKey::F7: return SDL_SCANCODE_F7;
		case dae::Keycodes::KeyboardKey::F8: return SDL_SCANCODE_F8;
		case dae::Keycodes::KeyboardKey::F9: return SDL_SCANCODE_F9;
		case dae::Keycodes::KeyboardKey::F10: return SDL_SCANCODE_F10;
		case dae::Keycodes::KeyboardKey::F11: return SDL_SCANCODE_F11;
		case dae::Keycodes::KeyboardKey::F12: return SDL_SCANCODE_F12;

		default: return SDL_SCANCODE_UNKNOWN;
	}
}



// Base class implementation
void dae::Keyboard::Update()
{
	m_pImpl->Update();
}

bool dae::Keyboard::GetButton(int button) const
{
	return  m_pImpl->GetButton(button);
}

bool dae::Keyboard::GetButtonDown(int button) const
{
	return  m_pImpl->GetButtonDown(button);
}

bool dae::Keyboard::GetButtonUp(int button) const
{
	return  m_pImpl->GetButtonUp(button);
}

dae::Keyboard::Keyboard()
	: m_pImpl{ std::make_unique<KeyboardImpl>() }
{
}

dae::Keyboard::~Keyboard() = default;

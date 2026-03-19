#include "InputHandling/Keyboard.h"

#include "InputHandling/InputCodes.h"
#include <SDL3/SDL.h>
#include <array>

// SDL Implementation
class mg::Keyboard::KeyboardImpl
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

bool  mg::Keyboard::KeyboardImpl::GetButton(int button) const
{
	return m_current[button];
}

bool  mg::Keyboard::KeyboardImpl::GetButtonDown(int button) const
{
	return m_current[button] && !m_previous[button];
}

bool  mg::Keyboard::KeyboardImpl::GetButtonUp(int button) const
{
	return !m_current[button] && m_previous[button];
}

void mg::Keyboard::KeyboardImpl::Update()
{
	SDL_PumpEvents();

	m_previous = m_current;

	const bool* sdlState = SDL_GetKeyboardState(nullptr);
	for (size_t keyIdx{}; keyIdx < m_current.size(); ++keyIdx)
	{
		auto stateIdxKey = KeyToSDL(static_cast<mg::Keycodes::KeyboardKey>(keyIdx));
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

mg::Keyboard::KeyboardImpl::KeyboardImpl()
	: m_current{}
	, m_previous{}
{
}

SDL_Scancode mg::Keyboard::KeyboardImpl::KeyToSDL(mg::Keycodes::KeyboardKey key) const
{
	switch (key)
	{
		case mg::Keycodes::KeyboardKey::A: return SDL_SCANCODE_A;
		case mg::Keycodes::KeyboardKey::B: return SDL_SCANCODE_B;
		case mg::Keycodes::KeyboardKey::C: return SDL_SCANCODE_C;
		case mg::Keycodes::KeyboardKey::D: return SDL_SCANCODE_D;
		case mg::Keycodes::KeyboardKey::E: return SDL_SCANCODE_E;
		case mg::Keycodes::KeyboardKey::F: return SDL_SCANCODE_F;
		case mg::Keycodes::KeyboardKey::G: return SDL_SCANCODE_G;
		case mg::Keycodes::KeyboardKey::H: return SDL_SCANCODE_H;
		case mg::Keycodes::KeyboardKey::I: return SDL_SCANCODE_I;
		case mg::Keycodes::KeyboardKey::J: return SDL_SCANCODE_J;
		case mg::Keycodes::KeyboardKey::K: return SDL_SCANCODE_K;
		case mg::Keycodes::KeyboardKey::L: return SDL_SCANCODE_L;
		case mg::Keycodes::KeyboardKey::M: return SDL_SCANCODE_M;
		case mg::Keycodes::KeyboardKey::N: return SDL_SCANCODE_N;
		case mg::Keycodes::KeyboardKey::O: return SDL_SCANCODE_O;
		case mg::Keycodes::KeyboardKey::P: return SDL_SCANCODE_P;
		case mg::Keycodes::KeyboardKey::Q: return SDL_SCANCODE_Q;
		case mg::Keycodes::KeyboardKey::R: return SDL_SCANCODE_R;
		case mg::Keycodes::KeyboardKey::S: return SDL_SCANCODE_S;
		case mg::Keycodes::KeyboardKey::T: return SDL_SCANCODE_T;
		case mg::Keycodes::KeyboardKey::U: return SDL_SCANCODE_U;
		case mg::Keycodes::KeyboardKey::V: return SDL_SCANCODE_V;
		case mg::Keycodes::KeyboardKey::W: return SDL_SCANCODE_W;
		case mg::Keycodes::KeyboardKey::X: return SDL_SCANCODE_X;
		case mg::Keycodes::KeyboardKey::Y: return SDL_SCANCODE_Y;
		case mg::Keycodes::KeyboardKey::Z: return SDL_SCANCODE_Z;

		case mg::Keycodes::KeyboardKey::Num0: return SDL_SCANCODE_0;
		case mg::Keycodes::KeyboardKey::Num1: return SDL_SCANCODE_1;
		case mg::Keycodes::KeyboardKey::Num2: return SDL_SCANCODE_2;
		case mg::Keycodes::KeyboardKey::Num3: return SDL_SCANCODE_3;
		case mg::Keycodes::KeyboardKey::Num4: return SDL_SCANCODE_4;
		case mg::Keycodes::KeyboardKey::Num5: return SDL_SCANCODE_5;
		case mg::Keycodes::KeyboardKey::Num6: return SDL_SCANCODE_6;
		case mg::Keycodes::KeyboardKey::Num7: return SDL_SCANCODE_7;
		case mg::Keycodes::KeyboardKey::Num8: return SDL_SCANCODE_8;
		case mg::Keycodes::KeyboardKey::Num9: return SDL_SCANCODE_9;

		case mg::Keycodes::KeyboardKey::Space: return SDL_SCANCODE_SPACE;
		case mg::Keycodes::KeyboardKey::Enter: return SDL_SCANCODE_RETURN;
		case mg::Keycodes::KeyboardKey::Escape: return SDL_SCANCODE_ESCAPE;
		case mg::Keycodes::KeyboardKey::Tab: return SDL_SCANCODE_TAB;
		case mg::Keycodes::KeyboardKey::Backspace: return SDL_SCANCODE_BACKSPACE;

		case mg::Keycodes::KeyboardKey::Up: return SDL_SCANCODE_UP;
		case mg::Keycodes::KeyboardKey::Down: return SDL_SCANCODE_DOWN;
		case mg::Keycodes::KeyboardKey::Left: return SDL_SCANCODE_LEFT;
		case mg::Keycodes::KeyboardKey::Right: return SDL_SCANCODE_RIGHT;

		case mg::Keycodes::KeyboardKey::LShift: return SDL_SCANCODE_LSHIFT;
		case mg::Keycodes::KeyboardKey::RShift: return SDL_SCANCODE_RSHIFT;
		case mg::Keycodes::KeyboardKey::LCtrl: return SDL_SCANCODE_LCTRL;
		case mg::Keycodes::KeyboardKey::RCtrl: return SDL_SCANCODE_RCTRL;
		case mg::Keycodes::KeyboardKey::LAlt: return SDL_SCANCODE_LALT;
		case mg::Keycodes::KeyboardKey::RAlt: return SDL_SCANCODE_RALT;

		case mg::Keycodes::KeyboardKey::F1: return SDL_SCANCODE_F1;
		case mg::Keycodes::KeyboardKey::F2: return SDL_SCANCODE_F2;
		case mg::Keycodes::KeyboardKey::F3: return SDL_SCANCODE_F3;
		case mg::Keycodes::KeyboardKey::F4: return SDL_SCANCODE_F4;
		case mg::Keycodes::KeyboardKey::F5: return SDL_SCANCODE_F5;
		case mg::Keycodes::KeyboardKey::F6: return SDL_SCANCODE_F6;
		case mg::Keycodes::KeyboardKey::F7: return SDL_SCANCODE_F7;
		case mg::Keycodes::KeyboardKey::F8: return SDL_SCANCODE_F8;
		case mg::Keycodes::KeyboardKey::F9: return SDL_SCANCODE_F9;
		case mg::Keycodes::KeyboardKey::F10: return SDL_SCANCODE_F10;
		case mg::Keycodes::KeyboardKey::F11: return SDL_SCANCODE_F11;
		case mg::Keycodes::KeyboardKey::F12: return SDL_SCANCODE_F12;

		default: return SDL_SCANCODE_UNKNOWN;
	}
}



// Base class implementation
void mg::Keyboard::Update()
{
	m_pImpl->Update();
}

bool mg::Keyboard::GetButton(int button) const
{
	return  m_pImpl->GetButton(button);
}

bool mg::Keyboard::GetButtonDown(int button) const
{
	return  m_pImpl->GetButtonDown(button);
}

bool mg::Keyboard::GetButtonUp(int button) const
{
	return  m_pImpl->GetButtonUp(button);
}

mg::Keyboard::Keyboard()
	: m_pImpl{ std::make_unique<KeyboardImpl>() }
{
}

mg::Keyboard::~Keyboard() = default;

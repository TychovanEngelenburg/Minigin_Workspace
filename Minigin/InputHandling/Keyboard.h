#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "InputHandling/InputDevice.h"

#include <memory>

namespace dae
{
	class Keyboard final : public InputDevice
	{
	public:
		bool GetButton(int button) const;
		bool GetButtonDown(int button) const;
		bool GetButtonUp(int button) const;

		void Update();

		Keyboard();

		~Keyboard();
		Keyboard(Keyboard const& other) = delete;
		Keyboard(Keyboard&& other) = delete;
		Keyboard& operator=(Keyboard const& other) = delete;
		Keyboard& operator=(Keyboard&& other) = delete;

	private:
		class KeyboardImpl;
		std::unique_ptr<KeyboardImpl> m_pImpl;
	};
};
#endif // !KEYBOARD_H

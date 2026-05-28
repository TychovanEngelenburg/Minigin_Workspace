#ifndef SDL_GAMEPAD_H
#define SDL_GAMEPAD_H
#include "Minigin/InputHandling/IInputDevice.h"
#include <memory>

namespace mg
{
	class SDLGamepad final : public IInputDevice
	{
	public:
		bool GetButton(int button) const override;
		bool GetButtonUp(int button) const override;
		bool GetButtonDown(int button) const override;
		// TODO: GetAxis() polling implementation.

		void Update();

		explicit SDLGamepad(int index);

		~SDLGamepad();
		SDLGamepad(SDLGamepad const& other) = delete;
		SDLGamepad(SDLGamepad&& other) = delete;
		SDLGamepad& operator=(SDLGamepad const& other) = delete;
		SDLGamepad& operator=(SDLGamepad&& other) = delete;

	private:
		class GamepadImpl;
		std::unique_ptr<GamepadImpl> m_pImpl;
	};
};
#endif // !SDL_GAMEPAD_H

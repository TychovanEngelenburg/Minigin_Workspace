#ifndef INPUTCODES_H
#define INPUTCODES_H

namespace dae
{
	namespace Keycodes
	{
		enum class KeyboardKey
		{
			Unknown,

			A, B, C, D, E, F, G,
			H, I, J, K, L, M, N,
			O, P, Q, R, S, T,
			U, V, W, X, Y, Z,

			Num0, Num1, Num2, Num3, Num4,
			Num5, Num6, Num7, Num8, Num9,

			Space,
			Enter,
			Escape,
			Tab,
			Backspace,

			Up,
			Down,
			Left,
			Right,

			LShift,
			RShift,
			LCtrl,
			RCtrl,
			LAlt,
			RAlt,

			F1, F2, F3, F4, F5, F6,
			F7, F8, F9, F10, F11, F12,

			KeyCount
		};

		enum class GamepadButton
		{
			DPadUp,
			DPadDown,
			DPadLeft,
			DPadRight,

			Start,
			Back,

			LeftStick,
			RightStick,

			LeftShoulder,
			RightShoulder,

			A,
			B,
			X,
			Y,

			ButtonCount
		};

		enum class GamepadAxis
		{
			LeftTrigger,
			RightTrigger,
			LeftStickX,
			LeftStickY,
			RightStickX,
			RightStickY,

			AxisCount
		};
	};
};
#endif // !INPUTCODES_H
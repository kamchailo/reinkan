#pragma once

#include <array>

#include "InputConstant.h"

namespace Reinkan::Input
{

	typedef uint32_t KeyCode;

	enum KeyState
	{
		Released,
		Pressed,
		Hold
	};

	class InputSystem
	{
	public:
		void Init();

		void Update();

		void Destroy();

		bool IsKeyPressed(KeyCode keyCode);

		bool IsKeyRelease(KeyCode keyCode);

		bool IsKeyHeld(KeyCode keyCode);

		bool IsMouseButtonPressed(KeyCode mouseButton);

		bool IsMouseButtonRelease(KeyCode mouseButton);

		bool IsMouseButtonHeld(KeyCode mouseButton);

	private:
		static std::unique_ptr<InputSystem> inputInstance;

		std::array<KeyState, MAX_KEY_NUMBER> KeyStateArray;

		std::array<KeyState, MAX_MOUSE_BUTTON_NUMBER> mouseButtonStateArray;
	};


}

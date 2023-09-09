#include "pch.h"
#include "InputSystem.h"

#include <glm/vec2.hpp>

namespace Reinkan::Input
{
	void InputSystem::Init()
	{
	}

	void InputSystem::Update()
	{

	}

	void InputSystem::Destroy()
	{
	}

	bool InputSystem::IsKeyPressed(KeyCode keyCode)
	{
		return (KeyStateArray[keyCode] == KeyState::Pressed);
	}

	bool InputSystem::IsKeyRelease(KeyCode keyCode)
	{
		return (KeyStateArray[keyCode] == KeyState::Released);
	}

	bool InputSystem::IsKeyHeld(KeyCode keyCode)
	{
		return (KeyStateArray[keyCode] == KeyState::Hold);
	}

	bool InputSystem::IsMouseButtonPressed(MousButtonCode mouseButton)
	{
		return (mouseButtonStateArray[mouseButton] == KeyState::Pressed);
	}

	bool InputSystem::IsMouseButtonRelease(MousButtonCode mouseButton)
	{
		return (mouseButtonStateArray[mouseButton] == KeyState::Released);
	}

	bool InputSystem::IsMouseButtonHeld(MousButtonCode mouseButton)
	{
		return (mouseButtonStateArray[mouseButton] == KeyState::Hold);
	}

	void InputSystem::KeyCallBack(KeyCode keyCode, KeyState action)
	{
		// Lookup key in case we use engine custome keyCode
		// KeyCode keyCode = GetKeyCode(key);
		if (action == Released)
		{
			KeyStateArray[keyCode] = KeyState::Released;
			return;
		}

		if (KeyStateArray[keyCode] == KeyState::Pressed || KeyStateArray[keyCode] == KeyState::Hold)
		{
			KeyStateArray[keyCode] = KeyState::Hold;
		}
		else
		{
			KeyStateArray[keyCode] = KeyState::Pressed;
		}
	}

	void InputSystem::MouseButtonCallBack(MousButtonCode mouseButtonCode, KeyState action)
	{
		if (action == Released)
		{
			KeyStateArray[mouseButtonCode] = KeyState::Released;
			return;
		}

		if (KeyStateArray[mouseButtonCode] == KeyState::Pressed || KeyStateArray[mouseButtonCode] == KeyState::Hold)
		{
			KeyStateArray[mouseButtonCode] = KeyState::Hold;
		}
		else
		{
			KeyStateArray[mouseButtonCode] = KeyState::Pressed;
		}
	}

	void InputSystem::SetMousePosition(const double x, const double y)
	{
		mousePosition = glm::vec2(x, y);
	}

	glm::vec2 InputSystem::GetMousePosition() const
	{
		return mousePosition;
	}

	glm::vec2 InputSystem::GetMouseDelta() const
	{
		return mouseDelta;
	}

	void InputSystem::MouseMove(const double x, const double y)
	{
		mouseDelta.x = mousePosition.x - x;
		mouseDelta.y = mousePosition.y - y;

		mousePosition.x = x;
		mousePosition.y = y;
	}
}


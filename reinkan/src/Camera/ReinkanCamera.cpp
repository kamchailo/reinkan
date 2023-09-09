#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

#include <GLFW/glfw3.h>
#include <cmath>

#include "Core/Locator/InputSystemLocator.h"

namespace Reinkan::Graphics
{
	void ReinkanApp::SetEyePosition(float eyeX, float eyeY, float eyeZ)
	{
		appEyePosition = glm::vec3(eyeX, eyeY, eyeZ);
	}

	void ReinkanApp::SetMainCamera(Camera::Camera* camera)
	{
		appMainCamera = camera;
	}

	void ReinkanApp::UpdateCamera(double deltaTime)
	{
		auto inputSystem = Core::InputSystemLocator::GetInputSystem();

		float speed = appMainCamera->GetSpeed() * deltaTime;

		float rad = 3.14159 / 180;

		if (inputSystem->IsKeyHeld(GLFW_KEY_W))
		{
			appMainCamera->Move(speed * glm::vec3(std::sin(appMainCamera->GetSpin() * rad), 0.0, -std::cos(appMainCamera->GetSpin() * rad)));
		}

		if (inputSystem->IsKeyHeld(GLFW_KEY_S))
		{
			appMainCamera->Move(-speed * glm::vec3(std::sin(appMainCamera->GetSpin() * rad), 0.0, -std::cos(appMainCamera->GetSpin() * rad)));
		}

		if (inputSystem->IsKeyHeld(GLFW_KEY_A))
		{
			appMainCamera->Move(-speed * glm::vec3(-std::cos(appMainCamera->GetSpin() * rad), 0.0, std::sin(appMainCamera->GetSpin() * rad)));
		}

		if (inputSystem->IsKeyHeld(GLFW_KEY_D))
		{
			appMainCamera->Move(speed * glm::vec3(std::cos(appMainCamera->GetSpin() * rad), 0.0, std::sin(appMainCamera->GetSpin() * rad)));
		}

		if (inputSystem->IsKeyHeld(GLFW_KEY_E))
		{
			appMainCamera->Move(speed * glm::vec3(0.0, 1.0, 0.0));
		}
		else if (inputSystem->IsKeyHeld(GLFW_KEY_C))
		{
			appMainCamera->Move(speed * glm::vec3(0.0, -1.0, 0.0));
		}

		if (inputSystem->IsMouseButtonHeld(GLFW_MOUSE_BUTTON_LEFT))
		{
			appMainCamera->SetSpin(appMainCamera->GetSpin() + inputSystem->GetMouseDelta().x);

			appMainCamera->SetTilt(appMainCamera->GetTilt() + inputSystem->GetMouseDelta().y);
		}

	}
}
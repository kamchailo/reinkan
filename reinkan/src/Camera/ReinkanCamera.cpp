#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <cmath>

#include "Core/Locator/TimeSystemLocator.h"
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

	void ReinkanApp::UpdateCamera()
	{
		if (!appMainCamera) return;

		Input::InputSystem* inputSystem = Core::InputSystemLocator::GetInputSystem();

		double deltaTime = Core::TimeSystemLocator::GetTime()->GetDeltaTime();

		float speed = appMainCamera->GetSpeed() * deltaTime;

		glm::vec3 direction = appMainCamera->GetDirection();

		glm::vec3 upDirection = appMainCamera->GetUp();

		float rad = 3.14159 / 180;

		if (inputSystem->IsKeyHeld(GLFW_KEY_W))
		{
			appMainCamera->Move(speed * direction);
			std::printf("direction x:%f, y:%f, z:%f | speed = %f\n", direction.x, direction.y, direction.z, glm::length(direction));
		}

		if (inputSystem->IsKeyHeld(GLFW_KEY_S))
		{
			appMainCamera->Move(speed * -direction);
		}

		if (inputSystem->IsKeyHeld(GLFW_KEY_A))
		{
			glm::vec3 sideDirection = glm::cross(direction, upDirection);
			appMainCamera->Move(speed * -sideDirection);
		}

		if (inputSystem->IsKeyHeld(GLFW_KEY_D))
		{
			glm::vec3 sideDirection = glm::cross(direction, upDirection);
			appMainCamera->Move(speed * sideDirection);
		}

		if (inputSystem->IsKeyHeld(GLFW_KEY_E))
		{
			appMainCamera->Move(speed * upDirection);
		}
		else if (inputSystem->IsKeyHeld(GLFW_KEY_C))
		{
			appMainCamera->Move(speed * -upDirection);
		}

		if (inputSystem->IsMouseButtonHeld(GLFW_MOUSE_BUTTON_LEFT))
		{
			// Change back to spin

			glm::vec4 directionVector = glm::vec4(appMainCamera->GetDirection(), 1.0);

			glm::vec4 rotatedDirection = glm::rotate(glm::mat4(1), glm::radians(appMainCamera->GetRotateSpeed() * inputSystem->GetMouseDelta().x), glm::vec3(0.0, 1.0, 0.0)) * directionVector;
			rotatedDirection = glm::rotate(glm::mat4(1), glm::radians(appMainCamera->GetRotateSpeed() * inputSystem->GetMouseDelta().y), glm::vec3(1.0, 0.0, 0.0)) * rotatedDirection;
			
			//glm::vec3 normalizedDirection = glm::normalize(rotatedDirection);

			appMainCamera->SetDirection(glm::vec3(rotatedDirection.x, rotatedDirection.y, rotatedDirection.z));
		}

		//appMainCamera->UpdateCursorPosition();

		float aspect = static_cast<float>(appSwapchainExtent.width) / static_cast<float>(appSwapchainExtent.height);

		appMainCamera->UpdatePerspectiveMatrix(aspect);

		appMainCamera->UpdateViewMatrix(deltaTime);

	}
	Camera::Camera* ReinkanApp::GetMainCamera() const
	{
		return appMainCamera;
	}
}
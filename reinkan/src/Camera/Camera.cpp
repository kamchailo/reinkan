#include "pch.h"
#include "Camera.h"

#include <glm/gtx/transform.hpp>

namespace Reinkan::Camera
{
	Camera::Camera() : position(glm::vec3(0.0,2.0,2.0)),
					    spin(1.57f), // PI / 2.0
					    tilt(0.0f),
						fieldOfView(45.0),
					    pFront(0.1f),
					    pBack(1000.0f),
					    rY(0.57),
						speed(0.7),
						rotateSpeed(0.5)
	{
		perspectiveMatrix = glm::mat4(1);
		viewMatrix = glm::mat4(1);

		cameraDirection = glm::normalize(glm::vec3(0.0, 0.0, -1.0));
		cameraUp = glm::vec3(0.0, 1.0, 0.0);
	}

	Camera::Camera(glm::vec3& position,
		float spin,
		float tilt,
		float fieldOfView,
		float pFront,
		float pBack,
		float pRy,
		float speed,
		float rotateSpeed)
		:
		position(position),
		spin(spin),
		tilt(tilt),
		fieldOfView(fieldOfView),
		pFront(pFront),
		pBack(pBack),
		rY(pRy),
		speed(speed),
		rotateSpeed(rotateSpeed)
	{
		perspectiveMatrix = glm::mat4(1);
		viewMatrix = glm::mat4(1);

		cameraDirection = glm::vec3(0.0, 0.0, -1.0);
		cameraUp = glm::vec3(0.0, 1.0, 0.0);
	}

	void Camera::UpdatePerspectiveMatrix(const float aspect)
	{
		perspectiveMatrix = glm::perspective(fieldOfView, aspect, pFront, pBack);
	}

	void Camera::UpdateViewMatrix(const float deltaTime)
	{
		viewMatrix = glm::lookAt(position, position + cameraDirection, glm::vec3(0.0, 1.0, 0.0));
	}

	void Camera::UpdateDirection()
	{
		glm::vec3 direction;
		direction.x = cos(spin) * -cos(tilt);
		direction.y = -sin(tilt);
		direction.z = sin(spin) * -cos(tilt);
		cameraDirection = glm::normalize(direction);
	}

	glm::mat4 Camera::GetPerspectiveMatrix() const
	{
		return perspectiveMatrix;
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return viewMatrix;
	}

	void Camera::Move(const glm::vec3& distance)
	{
		position += distance;
	}

	void Camera::Spin(float spinRad)
	{
		spin += spinRad;
	}

	void Camera::Tilt(float tiltRad)
	{
		tilt += tiltRad;
	}

	void Camera::UpdateCursorPosition(const float x, const float y)
	{
		cursorPosX = x;
		cursorPosY = y;
	}

	float Camera::GetSpeed() const
	{
		return speed;
	}

	float Camera::GetRotateSpeed() const
	{
		return rotateSpeed;
	}

	void Camera::SetDirection(glm::vec3 direction)
	{
		cameraDirection = direction;
	}

	glm::vec3 Camera::GetDirection() const
	{
		return cameraDirection;
	}

	glm::vec3 Camera::GetUp() const
	{
		return cameraUp;
	}

	void Camera::SetSpin(float spin)
	{
		this->spin = spin;
	}

	void Camera::SetTilt(float tilt)
	{
		this->tilt = tilt;
	}

	float Camera::GetSpin() const
	{
		return spin;
	}

	float Camera::GetTilt() const
	{
		return tilt;
	}

}

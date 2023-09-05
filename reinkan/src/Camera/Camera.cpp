#include "pch.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


namespace Reinkan
{
	Camera::Camera()
	{

	}

	Camera::Camera(glm::vec3& position,
		float spin,
		float tilt,
		float pFront,
		float pBack,
		float pRy)
		:
		position(position),
		spin(spin),
		tilt(tilt),
		pFront(pFront),
		pBack(pBack),
		rY(pRy)
	{

	}

	void Camera::Move(const glm::vec3& distance)
	{
		position += distance;
	}

	void Camera::UpdatePerspectiveMatrix(const float aspect)
	{
		float rX = rY * aspect;

		perspectiveMatrix[0][0] = 1.0 / rX;
		perspectiveMatrix[1][1] = -1.0 / rY; // Vulkan's Y point down
		perspectiveMatrix[2][2] = -pBack / (-pBack - pFront);
		perspectiveMatrix[3][2] = -(pFront * pBack) / (pBack - pFront);
		perspectiveMatrix[2][3] = -1;
		perspectiveMatrix[3][3] = 0;
	}

	void Camera::UpdateViewMatrix(const float time)
	{
		float currSpin = spin;
		float currTilt = tilt;
		glm::vec3 currEye = position;
		/*
		if (time < endTime) {
			//modified = true;
			float t = (time - startTime) / (endTime - startTime);
			//t = 2*t-t*t;
			t = (3 - 2 * t) * t * t;
			currSpin = (1 - t) * startSpin + t * spin;
			currTilt = (1 - t) * startTilt + t * tilt;
			currEye = (1 - t) * startEye + t * eye;
		}

		*/
		glm::mat4 spinMat = glm::rotate(currSpin * 3.14159f / 180.0f, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 tiltMat = glm::rotate(currTilt * 3.14159f / 180.0f, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 translateMat = glm::translate(-currEye);
		viewMatrix = tiltMat * spinMat * translateMat;
	}

	glm::mat4 Camera::GetPerspectiveMatrix(const float aspect) const
	{
		return perspectiveMatrix;
	}

	glm::mat4 Camera::GetViewMatrix(const float aspect) const
	{
		return viewMatrix;
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

}

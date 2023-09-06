#include "pch.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


namespace Reinkan
{
	Camera::Camera() : position(glm::vec3(0.0,2.0,2.0)),
					    spin(0.0f),
					    tilt(0.0f),
					    pFront(0.1f),
					    pBack(100.0f),
					    rY(0.57)
	{
		perspectiveMatrix = glm::mat4(1);
		viewMatrix = glm::mat4(1);
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
		perspectiveMatrix = glm::mat4(1);
		viewMatrix = glm::mat4(1);
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

	void Camera::UpdateViewMatrix(const float deltaTime)
	{
		float currSpin;
		float currTilt;
		glm::vec3 currEyePos;
		
		//modified = true;
		//float t = (time - startTime) / (endTime - startTime);
		//t = 2*t-t*t;
		//deltaTime = (3 - 2 * deltaTime) * deltaTime * deltaTime;
		//currSpin = (1 - deltaTime) * spin/*startSpin*/ + deltaTime * spin;
		//currTilt = (1 - deltaTime) * tilt/*startTilt*/ + deltaTime * tilt;
		//currEyePos = (1 - deltaTime) * position/*startEye*/ + deltaTime * position;
		
		//glm::mat4 spinMat = glm::rotate(currSpin * 3.14159f / 180.0f, glm::vec3(0.0, 1.0, 0.0));
		//glm::mat4 tiltMat = glm::rotate(currTilt * 3.14159f / 180.0f, glm::vec3(1.0, 0.0, 0.0));
		//glm::mat4 translateMat = glm::translate(-currEyePos);
		
		glm::mat4 spinMat = glm::rotate(spin * 3.14159f / 180.0f, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 tiltMat = glm::rotate(tilt * 3.14159f / 180.0f, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 translateMat = glm::translate(-position);
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

}

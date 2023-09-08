#pragma once

#include "glm/glm.hpp"


namespace Reinkan
{
	enum class CameraStateFlag 
	{
		None = 0x00,
		ToggleOrbit = 0x01, // for spin in first person
		TogglePan = 0x02
	};

	class Camera
	{
	public:
		Camera();

		Camera(glm::vec3& position,
				float spin,
				float tilt,
				float pFront,
				float pBack,
				float pRy);

		CameraStateFlag cameraStateFlag = CameraStateFlag::None;

		void UpdatePerspectiveMatrix(const float aspect);

		void UpdateViewMatrix(const float time);

		glm::mat4 GetPerspectiveMatrix(const float aspect) const;

		glm::mat4 GetViewMatrix(const float aspect) const;

		void Move(const glm::vec3& distance);

		void Spin(float spinRad);

		void Tilt(float tiltRad);

		void UpdateCursorPosition(const float x, const float y);

	private:
		glm::vec3 position = glm::vec3(0);

		float spin = 0.0f;

		float tilt = 0.0f;

		glm::mat4 perspectiveMatrix;

		glm::mat4 viewMatrix;

		float cursorPosX = 0.0f;

		float cursorPosY = 0.0f;

		float pFront = 0.1f;

		float pBack = 100.0f;

		float rY = 0.57f;
	};
}
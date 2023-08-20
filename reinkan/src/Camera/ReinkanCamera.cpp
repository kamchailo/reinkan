#include "pch.h"
#include "Reinkan.h"

namespace Reinkan
{
	void ReinkanApp::SetEyePosition(float eyeX, float eyeY, float eyeZ)
	{
		appEyePosition = glm::vec3(eyeX, eyeY, eyeZ);
	}
}
#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

namespace Reinkan::Graphics
{
	void ReinkanApp::SetEyePosition(float eyeX, float eyeY, float eyeZ)
	{
		appEyePosition = glm::vec3(eyeX, eyeY, eyeZ);
	}
}
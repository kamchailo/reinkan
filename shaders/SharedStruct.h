#pragma once

#include <glm/glm.hpp>

namespace Reinkan
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 texCoord;
	};

	struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};
}

#pragma once

#include <glm/glm.hpp>

namespace Reinkan
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 vertexNormal;
		glm::vec3 vertexTangent;
		glm::vec2 texCoord;
	};

	struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 viewInverse;
		alignas(16) glm::mat4 proj;
	};

	struct PushConstant
	{
		alignas(16) glm::mat4 modelMatrix;
		alignas(4) uint32_t objectId;
		alignas(4) uint32_t materialId;
	};

	struct Material {
		alignas(16) glm::vec3 diffuse;
		alignas(16) glm::vec3 specular;
		alignas(4) uint32_t normalMapId;
		alignas(4) uint32_t heightMapId;
		alignas(4) float shininess;
		alignas(4) uint32_t diffuseMapId;
	};


}

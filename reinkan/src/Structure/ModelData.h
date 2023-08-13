#pragma once

#include <vector>
#include <string>
#include <glm/mat4x4.hpp>

#include "../shaders/SharedStruct.h"

namespace Reinkan
{
	struct ModelData
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Material> materials;
		std::vector<uint32_t> materialIndex;
		std::vector<std::string> textures;
	};

	struct ModelDataLoading
	{
		std::shared_ptr<ModelData> modelDataPtr;
		glm::mat4 modelTransform;
	};
}
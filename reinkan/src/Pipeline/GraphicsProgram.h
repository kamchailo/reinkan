#pragma once

#include <vulkan/vulkan.h>

#include <string>
#include "Descriptor/DescriptorWrap.h"

namespace Reinkan
{
	class GraphicsProgram
	{
	public:

		GraphicsProgram(std::string vertexShaderPath,
			std::string fragmentShaderPath,
			DescriptorWrap descriptorWrap
			);

		VkPipelineLayout graphicPipelineLayout;

		VkPipeline graphicPipeline;


	private:

		void CreateGraphicPipelineLayout();

		void CreateGraphicPipelineLayout();
	};
}
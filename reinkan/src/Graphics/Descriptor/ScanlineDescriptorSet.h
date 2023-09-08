#pragma once

#include <vector>

#include "Graphics/ReinkanVulkan.h"

namespace Reinkan::Graphics
{
	class ScanlineDescriptorSet
	{
	public:

		std::vector<BufferWrap>     appMaterialBufferWraps;

		std::vector<ImageWrap>      appTextureImageWraps;
	};
}
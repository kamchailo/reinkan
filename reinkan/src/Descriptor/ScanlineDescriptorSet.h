#pragma once

#include <vector>

#include "Reinkan.h"

namespace Reinkan
{
	class ScanlineDescriptorSet
	{
	public:

		std::vector<BufferWrap>     appMaterialBufferWraps;

		std::vector<ImageWrap>      appTextureImageWraps;
	};
}
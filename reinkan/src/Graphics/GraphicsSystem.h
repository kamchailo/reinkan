#pragma once

#include "Graphics/ReinkanVulkan.h"

namespace Reinkan::Graphics
{
	class GraphicsSystem
	{
	public:
		void Init(uint32_t width, uint32_t height);

		void LoadResources();

		void Update();

		void Destroy();

		std::shared_ptr<ReinkanApp> GetVulkanApp() const;

	private:
		std::shared_ptr<ReinkanApp> vulkanApp;
	};
}
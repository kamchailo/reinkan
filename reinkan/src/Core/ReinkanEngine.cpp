#include "pch.h"
#include "ReinkanEngine.h"

#include "vulkan/vulkan.h"

#include "Core/Locator/TimeSystemLocator.h"
#include "Core/Locator/GraphicsSystemLocator.h"
#include "Core/Locator/InputSystemLocator.h"

namespace Reinkan::Core
{
	void ReinkanEngine::Init()
	{
		timeSystem = std::make_shared<Time::Time>();
		graphicsSystem = std::make_shared<Graphics::GraphicsSystem>();
		inputSystem = std::make_shared<Input::InputSystem>();

		TimeSystemLocator::Provide(timeSystem.get());
		GraphicsSystemLocator::Provide(graphicsSystem.get());
		InputSystemLocator::Provide(inputSystem.get());

		graphicsSystem->Init(1024, 768);
		inputSystem->Init();

		defaultCamera = std::make_unique<Camera::Camera>();
		graphicsSystem->GetVulkanApp()->SetMainCamera(defaultCamera.get());
	}

	void ReinkanEngine::Update()
	{
		try
		{
			Graphics::ReinkanApp* vulkanApp = graphicsSystem->GetVulkanApp();

			while (!vulkanApp->ShouldClose())
			{
				timeSystem->Update();

				inputSystem->Update();

				graphicsSystem->Update();

			}
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return;
		}

	}

	void ReinkanEngine::Destroy()
	{
		inputSystem->Destroy();

		graphicsSystem->Destroy();
	}
}


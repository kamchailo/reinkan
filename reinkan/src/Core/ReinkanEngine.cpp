#include "pch.h"
#include "ReinkanEngine.h"

#include "vulkan/vulkan.h"

#include "Core/Locator/GraphicsSystemLocator.h"
#include "Core/Locator/InputSystemLocator.h"

namespace Reinkan::Core
{
	void ReinkanEngine::Init()
	{
		graphicsSystem = std::make_shared<Graphics::GraphicsSystem>();
		inputSystem = std::make_shared<Input::InputSystem>();

		GraphicsSystemLocator::Provide(graphicsSystem.get());
		InputSystemLocator::Provide(inputSystem.get());

		graphicsSystem->Init(800, 600);
		inputSystem->Init();

		defaultCamera = std::make_unique<Camera::Camera>();
		graphicsSystem->GetVulkanApp()->SetMainCamera(defaultCamera.get());
	}

	void ReinkanEngine::Update()
	{
		try
		{
			while (!graphicsSystem->GetVulkanApp()->ShouldClose())
			{
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


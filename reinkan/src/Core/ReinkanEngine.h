#pragma once

#include "Time/Time.h"
#include "Graphics/GraphicsSystem.h"
#include "Input/InputSystem.h"

namespace Reinkan::Core
{
	class ReinkanEngine
	{
	public:
		ReinkanEngine() {};

		void Init();

		void Update();

		void Destroy();

	private:
		std::shared_ptr<Time::Time> timeSystem;

		std::shared_ptr<Graphics::GraphicsSystem> graphicsSystem;

		std::shared_ptr<Input::InputSystem> inputSystem;

		std::unique_ptr<Camera::Camera> defaultCamera;

	};
}
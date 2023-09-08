#pragma once

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
		std::shared_ptr<Reinkan::Graphics::GraphicsSystem> graphicsSystem;

		std::shared_ptr<Reinkan::Input::InputSystem> inputSystem;

	};
}
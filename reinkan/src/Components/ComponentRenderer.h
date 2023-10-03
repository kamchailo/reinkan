#pragma once

#include <memory>

#include "Component.h"
#include "Graphics/Structure/ObjectData.h"

namespace Reinkan::Component
{
	class ComponentRenderer : public Component
	{
	public:
		void Init() override;

		void Update() override;

		void Shutdown() override;

	private:

		std::shared_ptr<Graphics::ObjectData> objectData;

	};
}
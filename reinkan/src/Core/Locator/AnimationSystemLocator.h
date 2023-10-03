#pragma once

#include "Animation/AnimationSystem.h"

namespace Reinkan::Core
{
	class AnimationSystemLocator
	{
	public:
		static Animation::AnimationSystem* GetAnimationSystem()
		{
			return graphicsSystemInstance;
		}

		static void Provide(Animation::AnimationSystem* graphicsSystem)
		{
			graphicsSystemInstance = graphicsSystem;
		}

	private:
		inline static Animation::AnimationSystem* graphicsSystemInstance{ nullptr };
	};
}
#pragma once

#include <memory>

#include "GameObjects/GameObject.h"

namespace Reinkan::Component
{
	class Component
	{
	public:
		virtual ~Component() {};
		
		virtual void Init() = 0;
		
		virtual void Update() = 0;
		
		virtual void Shutdown() = 0;

		std::weak_ptr<GameObject::GameObject> GetGameObject() const
		{
			return gameObject;
		}
		
		void SetGameObject(std::weak_ptr<GameObject::GameObject> gameObject)
		{
			this->gameObject = gameObject;
		}

	protected:
		std::weak_ptr<GameObject::GameObject> gameObject;
	};
}
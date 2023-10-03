#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace Reinkan::Component
{
	class Component;
}

namespace Reinkan::GameObject
{
	class GameObject
	{
	public:
		GameObject();

		~GameObject();

		void Init();

		void Update();

		void Destroy();

		template<typename T>
		std::weak_ptr<T> AddComponent()
		{
			std::shared_ptr<Component::Component> component = std::make_shared<T>();

			components.push_back(component);

			component->gameObject = this;

			return component;
		}

		template<class T>
		std::weak_ptr<T> GetComponent()
		{
			for (auto component : components)
			{
				std::shared_ptr <T> castedComponent = std::dynamic_pointer_cast<T>(component);
				if (castedComponent != 0)
				{
					return castedComponent;
				}
			}
			return nullptr;
		}

	private:
		bool isDestroyed;

		glm::mat4 transformMatrix;

		std::weak_ptr<GameObject> parent;

		std::vector<std::weak_ptr<GameObject>> children;
			 
		std::vector<std::shared_ptr<Component::Component>> components;
	};
}
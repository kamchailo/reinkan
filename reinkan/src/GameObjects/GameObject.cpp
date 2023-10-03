#include "pch.h"
#include "GameObject.h"

#include "Components/Component.h"

namespace Reinkan::GameObject
{
	GameObject::GameObject()
	{

	}

	GameObject::~GameObject()
	{
		components.clear();
	}

	void GameObject::Init()
	{
		for (auto child : children)
		{
			child.lock()->Init();
		}
	}

	void GameObject::Update()
	{
		if (this->isDestroyed)
		{
			return;
		}

		for (auto component : components)
		{
			component->Update();
		}

		for (auto child : children)
		{
			child.lock()->Update();
		}
	}

	void GameObject::Destroy()
	{
		isDestroyed = true;
		for (auto child : children)
		{
			child.lock()->Destroy();
		}
	}

}


#include "Minigin/Scene/GameObject.h"
#include "Minigin/Scene/Scene.h"

#include <string_view>
#include <memory>
#include <glm/fwd.hpp>
#include <string>
#include <vector>

mg::Transform2D& mg::GameObject::Transform()
{
	return m_transform;
}


bool mg::GameObject::IsDestroyed() const noexcept
{
	return m_destroyed;
}

mg::Scene* mg::GameObject::Scene() const noexcept
{
	return m_pScene;
}

bool mg::GameObject::IsActive() const noexcept
{
	return m_active;
}


void mg::GameObject::SetScene(mg::Scene* pScene)
{
	m_pScene = pScene;

	for (size_t i = 0; i < Transform().ChildCount(); i++)
	{
		Transform().GetChildAt(i)->Owner().SetScene(pScene);
	}
}

void mg::GameObject::SetActive(bool isActive)
{
	m_active = isActive;

	for (size_t i = 0; i < Transform().ChildCount(); i++)
	{
		Transform().GetChildAt(i)->Owner().SetActive(isActive);
	}
}

void mg::GameObject::Destroy()
{
	m_destroyed = true;
	for (size_t i = 0; i < Transform().ChildCount(); i++)
	{
		Transform().GetChildAt(i)->Owner().Destroy();
	}
}

#pragma region Game_Loop
void mg::GameObject::Awake()
{
	for (auto& component : m_pComponents)
	{
		component->Awake();
	}
}

void mg::GameObject::Start()
{
	for (auto& component : m_pComponents)
	{
		component->Start();
	}
}

void mg::GameObject::OnCollisionEnter(mg::CollisionData const& data)
{
	for (auto& component : m_pComponents)
	{
		component->OnCollisionEnter(data);
	}
}

void mg::GameObject::OnCollisionStay(mg::CollisionData const& data)
{
	for (auto& component : m_pComponents)
	{
		component->OnCollisionStay(data);
	}
}

void mg::GameObject::OnCollisionExit(mg::CollisionData const& data)
{
	for (auto& component : m_pComponents)
	{
		component->OnCollisionExit(data);
	}
}

void mg::GameObject::Update()
{
	for (auto& component : m_pComponents)
	{
		if (component->IsActive())
		{
			component->Update();
		}
	}
}

void mg::GameObject::FixedUpdate()
{
	for (auto& component : m_pComponents)
	{
		if (component->IsActive())
		{
			component->FixedUpdate();
		}
	}
}

void mg::GameObject::Render() const
{
	for (auto& component : m_pComponents)
	{
		if (component->IsActive())
		{
			component->Render();
		}
	}
}

void mg::GameObject::LateUpdate()
{
	for (auto& component : m_pComponents)
	{
		if (component->IsActive())
		{
			component->LateUpdate();
		}
	}
}
#pragma endregion

mg::GameObject::GameObject(std::string_view name, glm::vec2 const& pos)
	: m_transform(*this)
	, Name(name)
{
	Transform().SetWorldPosition(pos);
}

mg::GameObject::~GameObject()
{
	m_pComponents.clear();
}

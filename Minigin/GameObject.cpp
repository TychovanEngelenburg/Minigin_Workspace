#include "Minigin/GameObject.h"

// .h includes
#include "EngineComponents/Component.h"
#include "Transform2D.h"

#include <string_view>
#include <memory>
#include <glm/fwd.hpp>
#include <string>
#include <vector>

mg::Transform2D& mg::GameObject::Transform()
{
	return m_transform;
}

std::string const& mg::GameObject::Name() const noexcept
{
	return m_name;
}

bool mg::GameObject::IsDestroyed() const noexcept
{
	return m_destroyed;
}

bool mg::GameObject::IsActive() const noexcept
{
	return m_active;
}

void mg::GameObject::SetActive(bool isActive)
{
	m_active = isActive;
}

void mg::GameObject::Destroy()
{
	m_destroyed = true;
	
	for (auto& child : m_pChildren)
	{
		child->Destroy();
	}
}

#pragma region Game_Loop
void mg::GameObject::Start()
{
	for (auto& component : m_pComponents)
	{
		component->Start();
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

void mg::GameObject::End()
{
	for (auto& component : m_pComponents)
	{
		component->End();
	}
}
#pragma endregion

mg::GameObject::GameObject(std::string_view name, glm::vec3 pos)
	: m_transform{this}
	, m_name{ name }
	, m_active{ true }
	, m_destroyed{}
	, m_pComponents{}
	, m_pParent{}
	, m_pChildren{}

{
	m_transform.SetLocalPosition(pos);
}

mg::GameObject::~GameObject()
{
	m_pComponents.clear();
}

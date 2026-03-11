#include "GameObject.h"

// .h includes
#include "EngineComponents/Component.h"
#include "Types/Transform.h"

#include <string_view>
#include <memory>
#include <glm/fwd.hpp>
#include <string>
#include <vector>

dae::Transform& dae::GameObject::GetTransform()
{
	return m_transform;
}

std::string const& dae::GameObject::GetName() const noexcept
{
	return m_name;
}

bool dae::GameObject::IsDestroyed() const noexcept
{
	return m_destroyed;
}

bool dae::GameObject::IsActive() const noexcept
{
	return m_active;
}

void dae::GameObject::SetActive(bool isActive)
{
	m_active = isActive;
}

void dae::GameObject::Destroy()
{
	m_destroyed = true;
	
	m_transform.DestroyChildren();
}

#pragma region Game_Loop
void dae::GameObject::Start()
{
	for (auto& component : m_components)
	{
		component->Start();
	}
}
void dae::GameObject::Update()
{
	for (auto& component : m_components)
	{
		if (component->IsActive())
		{
			component->Update();
		}
	}
}

void dae::GameObject::FixedUpdate()
{
	for (auto& component : m_components)
	{
		if (component->IsActive())
		{
			component->FixedUpdate();
		}
	}
}

void dae::GameObject::Render() const
{
	for (auto& component : m_components)
	{
		if (component->IsActive())
		{
			component->Render();
		}
	}
}

void dae::GameObject::LateUpdate()
{
	for (auto& component : m_components)
	{
		if (component->IsActive())
		{
			component->LateUpdate();
		}
	}
}

void dae::GameObject::End()
{
	for (auto& component : m_components)
	{
		component->End();
	}
}
#pragma endregion

dae::GameObject::GameObject(std::string_view name, glm::vec3 pos)
	: m_transform{ *this, pos }
	, m_name{ name }
	, m_active{ true }
	, m_destroyed{}
	, m_components{}

{
	m_transform.SetPosition(pos);
}

dae::GameObject::~GameObject()
{
	m_components.clear();
}


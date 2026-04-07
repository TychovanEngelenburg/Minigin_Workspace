#include "GameObject.h"

// .h includes
#include "EngineComponents/Component.h"
#include "Types/Transform.h"

#include <string_view>
#include <memory>
#include <glm/fwd.hpp>
#include <string>
#include <vector>

mg::Transform& mg::GameObject::GetTransform()
{
	return m_transform;
}

std::string const& mg::GameObject::GetName() const noexcept
{
	return m_name;
}

bool mg::GameObject::IsDestroyed() const noexcept
{
	return m_destroyed;
}

mg::GameObject* mg::GameObject::GetParent() const noexcept
{
	return m_pParent;
}

size_t mg::GameObject::GetChildCount() const noexcept
{
	return m_pChildren.size();
}

mg::GameObject* mg::GameObject::GetChildAt(size_t idx) const noexcept
{
	if (idx >= m_pChildren.size())
	{
		return nullptr;
	}
	return m_pChildren[idx];
}

bool mg::GameObject::HasChild(GameObject* pChild)
{
	if (std::find(m_pChildren.begin(), m_pChildren.end(), pChild) != m_pChildren.end())
	{
		return true;
	}

	return false;
}

bool mg::GameObject::IsChildOf(GameObject* pChild)
{
	if (m_pParent != nullptr)
	{
		if (m_pParent == pChild)
		{
			return true;
		}

		return m_pParent->IsChildOf(pChild);
	}

	return false;
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

void mg::GameObject::SetParent(GameObject* pParent, bool keepWorldPos)
{
	if (HasChild(pParent) || pParent == this || m_pParent == pParent)
	{
		return;
	}

	if (m_pParent)
	{
		m_pParent->RemoveChild(this);
	}

	m_pParent = pParent;
	m_transform.SetParent(&pParent->GetTransform(), keepWorldPos);

	if (m_pParent)
	{
		m_pParent->AddChild(this);
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

void mg::GameObject::AddChild(GameObject* pChild)
{
	if (!pChild)
	{
		return;
	}

	m_pChildren.emplace_back(pChild);
}

void mg::GameObject::RemoveChild(GameObject* pChild)
{
	std::erase(m_pChildren, pChild);
}

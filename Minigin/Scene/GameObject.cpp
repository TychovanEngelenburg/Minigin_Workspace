#include "GameObject.h"

#include "Minigin/Scene/Scene.h"

#include <string_view>
#include <memory>
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

bool mg::GameObject::ActiveSelf() const noexcept
{
	return m_active && !m_destroyed;
}

bool mg::GameObject::ActiveInHieriarchy() const
{
	if (!m_activeDirty)
	{
		return m_activeInHieriarchy;
	}

	bool active{ ActiveSelf()};
	

	if (auto parent = m_transform.Parent())
	{
		active = active && parent->Object().ActiveInHieriarchy();
	}

	m_activeInHieriarchy = active;
	m_activeDirty = false;
	return active;
}


void mg::GameObject::MarkActiveDirty()
{
	m_activeDirty = true;
	for (size_t i = 0; i < Transform().ChildCount(); i++)
	{
		m_transform.GetChildAt(i)->Object().MarkActiveDirty();
	}
}

/// <summary>
/// Set the scene pointer of this gameobject. This is only allowed to be used by Scene itself!
/// </summary>
/// <param name="pScene"></param>
void mg::GameObject::SetScene(mg::Scene* pScene)
{
	m_pScene = pScene;
}


void mg::GameObject::OnEnable()
{
	if (!m_awakened)
	{
		return;
	}

	if (!m_started)
	{
		Start();
	}

	for (size_t i = 0; i < Transform().ChildCount(); i++)
	{
		auto& child = m_transform.GetChildAt(i)->Object();

		if (child.ActiveSelf())
		{
			child.OnEnable();
		}
	}

	for (auto& component : m_pComponents)
	{
		if (component->EnabledSelf())
		{
			component->OnEnable();
		}
	}
}

void mg::GameObject::OnDisable()
{
	if (!m_awakened)
	{
		return;
	}

	for (size_t i = 0; i < Transform().ChildCount(); i++)
	{
		auto& child = m_transform.GetChildAt(i)->Object();

		if (child.ActiveSelf())
		{
			child.OnDisable();
		}
	}

	for (auto& component : m_pComponents)
	{
		if (!component->EnabledSelf())
		{
			continue;
		}

		component->OnDisable();
	}
}

void mg::GameObject::SetActive(bool active)
{
	if (m_active == active)
	{
		return;
	}


	bool wasActive{ ActiveInHieriarchy() };
	
	m_active = active;
	MarkActiveDirty();

	bool isActive{ ActiveInHieriarchy() };

	if (!wasActive && isActive)
	{
		OnEnable();
	}
	else if (wasActive && !isActive)
	{
		OnDisable();
	}
}

void mg::GameObject::Destroy()
{
	if (m_destroyed)
	{
		return;
	}

	for (size_t i = 0; i < m_transform.ChildCount(); i++)
	{
		m_transform.GetChildAt(i)->Object().Destroy();
	}

	SetActive(false);
	m_destroyed = true;
}

#pragma region Game_Loop
void mg::GameObject::Awake()
{
	m_awakened = true;

	for (auto& component : m_pComponents)
	{
		component->Awake();

		//if (component->ActiveAndEnabled())
		//{
		//	component->OnEnable();
		//}
	}

	if (ActiveInHieriarchy())
	{
		OnEnable();
	}
}

void mg::GameObject::Start()
{
	for (auto& component : m_pComponents)
	{
		component->Start();
	}
	m_started = true;
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

void mg::GameObject::FixedUpdate()
{
	for (auto& component : m_pComponents)
	{
		if (component->ActiveAndEnabled())
		{
			component->FixedUpdate();
		}
	}
}

void mg::GameObject::Update()
{
	for (auto& component : m_pComponents)
	{
		if (component->ActiveAndEnabled())
		{
			component->Update();
		}
	}
}


void mg::GameObject::Render() const
{
	for (auto& component : m_pComponents)
	{
		if (component->ActiveAndEnabled())
		{
			component->Render();
		}
	}
}

void mg::GameObject::LateUpdate()
{
	for (auto& component : m_pComponents)
	{
		if (component->ActiveAndEnabled())
		{
			component->LateUpdate();
		}
	}
}

void mg::GameObject::Cleanup()
{
	std::erase_if(m_pComponents,
		[&](auto const& component)
		{
			return std::find( m_pPendingRemoval.begin(), m_pPendingRemoval.end(), component.get()) != m_pPendingRemoval.end();
		});


	m_pPendingRemoval.clear();
}
#pragma endregion

mg::GameObject::GameObject(std::string_view name, glm::vec2 const& pos)
	: m_transform(*this)
	, Name(name)
{
	m_transform.SetWorldPosition(pos);
}

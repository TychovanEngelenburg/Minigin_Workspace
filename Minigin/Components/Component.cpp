#include "Component.h"

#include "Minigin/Scene/GameObject.h"
#include "Minigin/Collisions/CollisionSystem.h"

mg::GameObject* mg::Component::Object() const noexcept
{
	return m_pGameObject;
}

bool mg::Component::ActiveAndEnabled() const noexcept
{
	return m_enabled && Object()->ActiveInHieriarchy();
}

bool mg::Component::EnabledSelf() const noexcept
{
	return m_enabled;
}

void mg::Component::SetEnabled(bool enabled)
{
	if (m_enabled == enabled)
	{
		return;
	}

	m_enabled = enabled;

	if (Object()->ActiveInHieriarchy())
	{
		if (enabled)
		{
			OnEnable();
		}
		else
		{
			OnDisable();
		}
	}
}

#pragma region Game_Loop
void mg::Component::Awake() {}

void mg::Component::OnEnable() {}

void mg::Component::OnDisable() {}

void mg::Component::Start() {}

void mg::Component::OnCollisionEnter(mg::CollisionData const& data) {}

void mg::Component::OnCollisionStay(mg::CollisionData const& data) {}

void mg::Component::OnCollisionExit(mg::CollisionData const& data) {}

void mg::Component::FixedUpdate() {}

void mg::Component::Update() {}

void mg::Component::LateUpdate() {}

void mg::Component::Render() const {}
#pragma endregion Game_Loop

mg::Component::Component(mg::GameObject& owner)
	: m_pGameObject{ &owner }
	, m_enabled(true)
{
}

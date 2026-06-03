#include "Component.h"

#include "Minigin/Scene/GameObject.h"
#include "Minigin/Collisions/CollisionSystem.h"

mg::GameObject* mg::Component::Owner() const noexcept
{
	return m_pGameObject;
}

bool mg::Component::IsActive() const noexcept
{
	return m_active;
}

void mg::Component::SetActive(bool isActive)
{
	m_active = isActive;
}

#pragma region Game_Loop
void mg::Component::Awake() {}

void mg::Component::Start() {}

void mg::Component::OnCollisionEnter(mg::CollisionData const& data) {}

void mg::Component::OnCollisionStay(mg::CollisionData const& data) {}

void mg::Component::OnCollisionExit(mg::CollisionData const& data) {}

void mg::Component::FixedUpdate() {}

void mg::Component::Update() {}

void mg::Component::Render() const {}

void mg::Component::LateUpdate() {}
#pragma endregion Game_Loop

mg::Component::Component(mg::GameObject& owner)
	: m_pGameObject{ &owner }
	, m_active{ true }
{
}

#include "EngineComponents/Component.h"
#include "GameObject.h"

mg::GameObject* mg::Component::GetOwner() const noexcept
{
	return m_pOwner;
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
void mg::Component::Start() {}

void mg::Component::FixedUpdate() {}

void mg::Component::Update() {}

void mg::Component::Render() const {}

void mg::Component::LateUpdate() {}

void mg::Component::End() {}
#pragma endregion Game_Loop

mg::Component::Component(GameObject& owner)
	: m_pOwner{ &owner }
	, m_active{ true }
{
}

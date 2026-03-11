#include "EngineComponents/Component.h"
#include "GameObject.h"

dae::GameObject* dae::Component::GetOwner() const noexcept
{
	return m_owner;
}

bool dae::Component::IsActive() const noexcept
{
	return m_active;
}

void dae::Component::SetActive(bool isActive)
{
	m_active = isActive;
}

#pragma region Game_Loop
void dae::Component::Start() {}

void dae::Component::FixedUpdate() {}

void dae::Component::Update() {}

void dae::Component::Render() const {}

void dae::Component::LateUpdate() {}

void dae::Component::End() {}
#pragma endregion Game_Loop

dae::Component::Component(GameObject& owner)
	: m_owner{ &owner }
	, m_active{ true }
{
}

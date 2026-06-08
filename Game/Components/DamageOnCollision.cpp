#include "DamageOnCollision.h"

#include "Tank/TankHealth.h"

#include <Minigin/Collisions/CollisionSystem.h>
#include <Minigin/Scene/GameObject.h>

void DamageOnCollision::SetPlayerId(std::optional<int> playerId)
{
	m_playerId = playerId;
}

void DamageOnCollision::SetDamageAmount(int amount)
{
	m_damageAmount = amount;
}

void DamageOnCollision::OnCollisionEnter(mg::CollisionData const& data)
{
	if (auto const& healthComp{data.pOtherCollider->Object()->GetComponent<TankHealth>()})
	{
		healthComp->Damage(m_damageAmount, m_playerId);
	}
}

DamageOnCollision::DamageOnCollision(mg::GameObject& owner)
	: Component(owner)
{
}

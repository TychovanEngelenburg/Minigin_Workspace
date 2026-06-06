#include "DamageOnCollision.h"

#include "Tank/TankHealth.h"

#include <Minigin/Collisions/CollisionSystem.h>
#include <Minigin/Scene/GameObject.h>

void DamageOnCollision::SetKillerId(int id)
{
	m_killerId = id;
}

void DamageOnCollision::OnCollisionEnter(mg::CollisionData const& data)
{
	if (auto const& healthComp{data.pOtherCollider->Object()->GetComponent<TankHealth>()})
	{
		healthComp->Damage(m_damageAmount, m_killerId);
	}
}

void DamageOnCollision::SetDamageAmount(int amount)
{
	m_damageAmount = amount;
}

DamageOnCollision::DamageOnCollision(mg::GameObject& owner)
	: Component(owner)
{
}

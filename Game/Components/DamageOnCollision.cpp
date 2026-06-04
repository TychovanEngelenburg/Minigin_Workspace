#include "DamageOnCollision.h"

#include "Tank/TankHealth.h"

#include <Minigin/Collisions/CollisionSystem.h>
#include <Minigin/Scene/GameObject.h>

void DamageOnCollision::OnCollisionEnter(mg::CollisionData const& data)
{
	if (auto const& healthComp{data.pOtherCollider->Owner()->GetComponent<TankHealth>()})
	{
		healthComp->Damage(m_damageAmount);
	}
}

DamageOnCollision::DamageOnCollision(mg::GameObject& owner, int damageAmount)
	: Component(owner)
	, m_damageAmount(damageAmount)
{
}

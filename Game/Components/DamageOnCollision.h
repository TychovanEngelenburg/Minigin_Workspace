#ifndef DAMAGE_ON_COLLISION_H
#define DAMAGE_ON_COLLISION_H

#include <Minigin/Components/Component.h>

namespace mg
{
	class GameObject;
}

class DamageOnCollision final : public mg::Component
{
public:
	void OnCollisionEnter(mg::CollisionData const& data) override;

	DamageOnCollision(mg::GameObject& owner, int damageAmount);

private:
	int m_damageAmount;
};

#endif // !DAMAGE_ON_COLLISION_H

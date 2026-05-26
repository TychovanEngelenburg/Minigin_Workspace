#ifndef DAMAGE_ON_COLLISION_H
#define DAMAGE_ON_COLLISION_H
#include <Minigin/EngineComponents/Component.h>

namespace mg
{
	class GameObject;
}

class DamageOnCollision : public mg::Component
{
public:
	void OnCollisionEnter(mg::CollisionData const& data) override;

	DamageOnCollision(mg::GameObject& owner, int damageAmount);

private:
	int m_damageAmount;
};

#endif // !DAMAGE_ON_COLLISION_H

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
	void SetKillerId(int id);

	void OnCollisionEnter(mg::CollisionData const& data) override;
	void SetDamageAmount(int amount);

	DamageOnCollision(mg::GameObject& owner);

private:
	int m_damageAmount{1};
	int m_killerId{ -1 };
};

#endif // !DAMAGE_ON_COLLISION_H

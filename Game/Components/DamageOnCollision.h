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

	virtual ~DamageOnCollision() = default;
	DamageOnCollision(DamageOnCollision const& other) = delete;
	DamageOnCollision(DamageOnCollision&& other) = delete;
	DamageOnCollision& operator=(DamageOnCollision const& other) = delete;
	DamageOnCollision& operator=(DamageOnCollision&& other) = delete;

private:
	int m_damageAmount;
};

#endif // !DAMAGE_ON_COLLISION_H

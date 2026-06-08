#ifndef DAMAGE_ON_COLLISION_H
#define DAMAGE_ON_COLLISION_H

#include <Minigin/Components/Component.h>
#include <optional>

namespace mg
{
	class GameObject;
}

class DamageOnCollision final : public mg::Component
{
public:
	void SetPlayerId(std::optional<int> playerId);

	void OnCollisionEnter(mg::CollisionData const& data) override;
	void SetDamageAmount(int amount);

	DamageOnCollision(mg::GameObject& owner);


private:
	std::optional<int> m_playerId{ std::nullopt };
	int m_damageAmount{1};
};

#endif // !DAMAGE_ON_COLLISION_H

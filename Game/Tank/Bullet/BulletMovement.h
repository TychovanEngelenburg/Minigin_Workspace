#ifndef PLAYER_BULLET_H
#define PLAYER_BULLET_H

#include <Minigin/EngineComponents/Component.h>

#include <glm/vec2.hpp>

namespace mg
{
	class GameObject;
	class BoxCollider2D;
};

class GameGrid;
class BulletPool;

class BulletMovement final : public mg::Component
{
public:
	void SetPool(BulletPool* pPool);

	void Destroy();

	void Shoot(glm::vec2 const& pos, glm::vec2 const& dir);

	void Awake() override;
	
	void OnCollisionEnter(mg::CollisionData const& data) override;


	void  FixedUpdate() override;

	BulletMovement(mg::GameObject& owner, GameGrid& pGrid);

private:
	BulletPool* m_pPool{};

	mg::BoxCollider2D* m_pCollider{};
	GameGrid* m_pGrid;
	glm::ivec2 m_currentTile{};

	int m_maxBounces{5};
	int m_bounceCount{};

	glm::vec2 m_direction{1.f, 1.f};
	glm::vec2 m_speed{100.f};
};

#endif // !PLAYER_BULLET_H
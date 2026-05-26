#include "PlayerBullet.h"
#include "Grid/GameGrid.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/DeltaClock.h>
#include <Minigin/CollisionSystem/BoxCollider2D.h>
#include <cassert>
void BulletMovement::Shoot(glm::vec2 const& pos, glm::vec2 const& dir)
{
	m_direction = dir;
	Owner()->Transform().SetWorldPosition(pos);
	m_currentTile = m_pGrid->WorldToGrid(pos);
	m_bounceCount = 0;
	Owner()->SetActive(true);
}

void BulletMovement::Awake()
{
	m_pCollider = Owner()->GetComponent<mg::BoxCollider2D>();

	assert(m_pCollider && "Bullet gameobject must have a collider!");
}

void BulletMovement::FixedUpdate()
{
	auto worldPos = Owner()->Transform().WorldPosition();

	glm::vec2 displacement = m_direction * m_speed * static_cast<float>(mg::DeltaClock::FixedDeltaTime());

	bool bounced{ false };

	float xPos{ worldPos.x + displacement.x };
	if (displacement.x > 0)
	{
		xPos += m_pCollider->LocalBounds().size.x;
	}

	if (m_pGrid->WallAt(m_pGrid->WorldToGrid({xPos, worldPos.y })) ||
		m_pGrid->WallAt(m_pGrid->WorldToGrid({xPos, worldPos.y + m_pCollider->LocalBounds().size.y })))
	{
		m_direction.x = -m_direction.x;
		bounced = true;
	}
	else
	{
		worldPos.x += displacement.x;
	}

	float yPos{ worldPos.y + displacement.y };
	if (displacement.y > 0)
	{
		yPos += m_pCollider->LocalBounds().size.x;
	}

	if (m_pGrid->WallAt(m_pGrid->WorldToGrid({ worldPos.x,                                      yPos })) ||
		m_pGrid->WallAt(m_pGrid->WorldToGrid({ worldPos.x + m_pCollider->LocalBounds().size.x,  yPos })))
	{
		m_direction.y = -m_direction.y;
		bounced = true;
	}
	else
	{
		worldPos.y += displacement.y;
	}


	if (bounced)
	{
		++m_bounceCount;

		if (m_bounceCount >= m_maxBounces)
		{
			//Owner()->SetActive(false);
			m_bounceCount = 0;
		}
	}

	Owner()->Transform().SetWorldPosition(worldPos);

	m_currentTile = m_pGrid->WorldToGrid(worldPos);
}

BulletMovement::BulletMovement(mg::GameObject& owner, GameGrid* pGrid, float speed, int maxBounces)
	: mg::Component(owner)
	, m_pGrid{ pGrid }
	, m_maxBounces{ maxBounces }
	, m_speed{ speed }
{

}

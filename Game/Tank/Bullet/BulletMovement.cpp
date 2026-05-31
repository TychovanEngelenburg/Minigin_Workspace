#include "Tank/Bullet/BulletMovement.h"

#include "Grid/GameGrid.h"
#include "Tank/Bullet/BulletPool.h"
#include <Minigin/Scene/GameObject.h>
#include <Minigin/DeltaClock.h>
#include <Minigin/CollisionSystem/BoxCollider2D.h>

#include <cassert>

void BulletMovement::SetPool(BulletPool* pPool)
{
	m_pPool = pPool;
}

void BulletMovement::Destroy()
{
	if (m_pPool)
	{
		Owner()->SetActive(false);
		m_pPool->ReturnBullet(this);
	}
	else
	{
		Owner()->Destroy();
	}
}

void BulletMovement::Shoot(glm::vec2 const& pos, glm::vec2 const& dir)
{
	Owner()->Transform().SetWorldPosition(pos);

	m_currentTile = m_pGrid->WorldToGrid(pos);
	if (m_pGrid->WallAt(m_currentTile)
		|| m_pGrid->WallAt(m_pGrid->WorldToGrid({ pos.x + m_pCollider->LocalBounds().size.x, pos.y }))
		|| m_pGrid->WallAt(m_pGrid->WorldToGrid({ pos.x , pos.y + m_pCollider->LocalBounds().size.y }))
		|| m_pGrid->WallAt(m_pGrid->WorldToGrid({ pos.x + m_pCollider->LocalBounds().size.x, pos.y + m_pCollider->LocalBounds().size.y})) )
	{
		Destroy();
	}

	m_direction = dir;
	m_bounceCount = 0;

	Owner()->SetActive(true);
}

void BulletMovement::Awake()
{
	m_pCollider = Owner()->GetComponent<mg::BoxCollider2D>();

	assert(m_pCollider && "Bullet gameobject must have a collider!");
}

void BulletMovement::OnCollisionEnter(mg::CollisionData const&)
{
	Destroy();
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

	if (m_pGrid->WallAt(m_pGrid->WorldToGrid({ xPos, worldPos.y })) ||
		m_pGrid->WallAt(m_pGrid->WorldToGrid({ xPos, worldPos.y + m_pCollider->LocalBounds().size.y })))
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
			Destroy();
		}
	}

	Owner()->Transform().SetWorldPosition(worldPos);

	m_currentTile = m_pGrid->WorldToGrid(worldPos);
}

BulletMovement::BulletMovement(mg::GameObject& owner, GameGrid& pGrid)
	: mg::Component(owner)
	, m_pGrid{ &pGrid }
{

}


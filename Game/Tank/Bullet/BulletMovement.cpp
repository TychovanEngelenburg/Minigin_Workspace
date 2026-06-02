#include "Tank/Bullet/BulletMovement.h"

#include "Grid/GameGrid.h"
#include "Tank/Bullet/BulletPool.h"
#include <Minigin/Scene/GameObject.h>
#include <Minigin/DeltaClock.h>
#include <Minigin/CollisionSystem/BoxCollider2D.h>
#include <Minigin/EngineComponents/Sprite.h>
#include <cassert>

mg::BoxCollider2D& BulletMovement::Collider() const noexcept
{
	return  *m_pCollider;
}

mg::Sprite& BulletMovement::Sprite() const noexcept
{
	return  *m_pSprite;
}

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

void BulletMovement::Activate(glm::vec2 const& pos, glm::vec2 const& dir)
{
	auto const halfSize{ m_pCollider->LocalBounds().size * 0.5f };
	m_currentTile = m_pGrid->WorldToGrid(pos);
	if (m_pGrid->WallAt(m_pGrid->WorldToGrid({ pos.x - halfSize.x, pos.y - halfSize.y })) ||
		m_pGrid->WallAt(m_pGrid->WorldToGrid({ pos.x + halfSize.x, pos.y - halfSize.y })) ||
		m_pGrid->WallAt(m_pGrid->WorldToGrid({ pos.x - halfSize.x, pos.y + halfSize.y })) ||
		m_pGrid->WallAt(m_pGrid->WorldToGrid({ pos.x + halfSize.x, pos.y + halfSize.y })))
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
	m_pSprite = Owner()->GetComponent<mg::Sprite>();

	assert(m_pCollider && "Bullet gameobject must have a collider!");
	assert(m_pSprite && "Bullet gameobject must have a sprite!");
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

	auto const halfSize = m_pCollider->LocalBounds().size * 0.5f;
	float xPos;

	if (displacement.x > 0.f)
	{
		xPos = worldPos.x + displacement.x + halfSize.x;
	}
	else
	{
		xPos = worldPos.x + displacement.x - halfSize.x;
	}

	if (m_pGrid->WallAt(m_pGrid->WorldToGrid({ xPos, worldPos.y - halfSize.y})) ||
		m_pGrid->WallAt(m_pGrid->WorldToGrid({ xPos, worldPos.y + halfSize.y})))
	{
		m_direction.x = -m_direction.x;
		bounced = true;
	}
	else
	{
		worldPos.x += displacement.x;
	}

	float yPos;
	if (displacement.y > 0.f)
	{
		yPos = worldPos.y + displacement.y + halfSize.y;
	}
	else
	{
		yPos = worldPos.y + displacement.y - halfSize.y;
	}

	if (m_pGrid->WallAt(m_pGrid->WorldToGrid({ worldPos.x - halfSize.x, yPos })) ||
		m_pGrid->WallAt(m_pGrid->WorldToGrid({ worldPos.x + halfSize.x, yPos })))
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


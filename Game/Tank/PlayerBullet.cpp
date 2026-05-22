#include "PlayerBullet.h"
#include "Grid/GameGrid.h"
#include "Minigin/Scene/GameObject.h"
#include "Minigin/DeltaClock.h"


void BulletMovement::Shoot(glm::vec2 const& pos, glm::vec2 const& dir)
{
	m_direction = dir;
	Owner()->Transform().SetWorldPosition(pos);
	m_currentTile = m_pGrid->WorldToGrid(pos);
	m_bounceCount = 0;
	Owner()->SetActive(true);
}

void BulletMovement::FixedUpdate()
{
    auto worldPos = Owner()->Transform().WorldPosition();

    const float dt =
        static_cast<float>(mg::DeltaClock::FixedDeltaTime());

    glm::vec2 displacement = m_direction * m_speed * dt;

    bool bounced{ false };


    glm::vec2 xPos
    {
        worldPos.x + displacement.x,
        worldPos.y
    };

    auto xTile = m_pGrid->WorldToGrid(xPos);

    if (m_pGrid->WallAt(xTile))
    {
        m_direction.x = -m_direction.x;
        bounced = true;
    }
    else
    {
        worldPos.x = xPos.x;
    }

    glm::vec2 yPos
    {
        worldPos.x,
        worldPos.y + displacement.y
    };

    auto yTile = m_pGrid->WorldToGrid(yPos);

    if (m_pGrid->WallAt(yTile))
    {
        m_direction.y = -m_direction.y;
        bounced = true;
    }
    else
    {
        worldPos.y = yPos.y;
    }

    if (bounced)
    {
        ++m_bounceCount;
    }

    Owner()->Transform().SetWorldPosition(worldPos);

    m_currentTile = m_pGrid->WorldToGrid(worldPos);
}

BulletMovement::BulletMovement(mg::GameObject& owner, GameGrid* pGrid, float speed)
	: mg::Component(owner)
	, m_pGrid{ pGrid }
	, m_speed{speed}
{

}

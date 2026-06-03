#include "AutoShooting.h"

#include "Game/Grid/GameGrid.h"
#include "Game/Tank/TankManager.h"
#include "Game/Tank/TankBarrel.h"

#include <Minigin/Scene/Scene.h>
#include <Minigin/Scene/GameObject.h>
#include <Minigin/CollisionSystem/CollisionSystem.h>

void AutoShooting::Awake()
{
	m_pBarrel = Owner()->GetComponent<TankBarrel>();
	assert(m_pBarrel && "MovementAI requires TankMovement");
}

void AutoShooting::Update()
{
	if (!m_pBarrel->OnCooldown())
	{
		if (CheckLineOfSight())
		{
			m_pBarrel->Shoot();
		}
	}
}

AutoShooting::AutoShooting(mg::GameObject& owner, TankManager& tankManager, GameGrid& grid)
	: Component(owner)
	, m_pTankmanager(&tankManager)
	, m_pGrid(&grid)
{

}

bool AutoShooting::CheckLineOfSight()
{
	auto angle = glm::radians(Owner()->Transform().WorldRotationZ());

	glm::vec2 forwardDir
	{
		cos(angle),
		sin(angle)
	};


	glm::vec2 pos
	{
		Owner()->Transform().WorldPosition()
	};

	float rayDistance{};
	for (; rayDistance < m_viewDistance; rayDistance += m_viewCheckStep)
	{
		glm::vec2 point = pos + forwardDir * rayDistance;

		if (m_pGrid->WallAt(m_pGrid->WorldToGrid(point)))
		{
			break;
		}
	}

	return Owner()->Scene()->CollisionSystem().Raycast(pos, forwardDir, rayDistance, m_pBarrel->Config().Collisions.LayerMask).Hit;
}

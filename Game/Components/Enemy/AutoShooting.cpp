#include "AutoShooting.h"

#include "Game/Components/System/GameGrid.h"
#include "Game/Components/System/TankManager.h"
#include "Game/Components/Tank/TankBarrel.h"

#include <glm/trigonometric.hpp>
#include <Minigin/Scene/Scene.h>
#include <Minigin/Scene/GameObject.h>
#include <Minigin/Collisions/CollisionSystem.h>

void AutoShooting::Awake()
{
	m_pBarrel = Object()->GetComponent<TankBarrel>();
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
	auto angle = glm::radians(Object()->Transform().WorldRotationZ());

	glm::vec2 forwardDir
	{
		cos(angle),
		sin(angle)
	};


	glm::vec2 pos
	{
		Object()->Transform().WorldPosition()
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

	return Object()->Scene()->CollisionSystem().Raycast(pos, forwardDir, rayDistance, m_pBarrel->Config().Collisions.LayerMask).Hit;
}

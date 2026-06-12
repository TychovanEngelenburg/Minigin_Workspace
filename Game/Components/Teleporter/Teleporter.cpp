#include "Teleporter.h"

#include "Game/Components/System/GameGrid.h"
#include "Game/Components/Tank/TankMovement.h"
#include "Game/Components/System/TankManager.h"

#include <Minigin/Collisions/CollisionSystem.h>
#include <Minigin/Scene/GameObject.h>

#include <algorithm>

Teleporter::Teleporter(mg::GameObject& owner, GameGrid& grid, TankManager& tankManager)
	: Component(owner)
	, m_pGrid(&grid)
	, m_pTankManager(&tankManager)
{
}

void Teleporter::Start()
{
	Object()->Transform().SetWorldPosition(m_pGrid->GridToWorld(m_pGrid->TeleporterPos()));
}

void Teleporter::OnCollisionEnter(mg::CollisionData const& data)
{
	if (auto* tank = data.pOtherCollider->Object()->GetComponent<TankMovement>())
	{	
		if (auto dest = FindRandomDestination(); dest.has_value())
		{
			tank->Teleport(dest.value());
		}
	}
}

std::optional<glm::ivec2> Teleporter::FindRandomDestination() const
{
	std::vector<glm::ivec2> freeTiles;

	auto tankOccupied = m_pTankManager->OccupiedTiles();
	auto const teleporterPos = m_pGrid->TeleporterPos();

	for (int y{}; y < m_pGrid->Size().y; ++y)
	{
		for (int x{}; x < m_pGrid->Size().x; ++x)
		{

			glm::ivec2 pos{ x, y };

			if (pos == teleporterPos ||
				!m_pGrid->IsPath(pos))
			{
				continue;
			}

			if (std::find(tankOccupied.begin(), tankOccupied.end(), pos) != tankOccupied.end())
			{
				continue;
			}

			freeTiles.push_back(pos);
		}
	}

	if (freeTiles.empty())
	{
		return std::nullopt;
	}

	return freeTiles[rand() % freeTiles.size()];
}
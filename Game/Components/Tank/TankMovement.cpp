#include "TankMovement.h"

#include "Game/Components/System/GameGrid.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/Core/DeltaClock.h>

#include <glm/geometric.hpp>

TankMovement::Direction const& TankMovement::CurrentDirection() const noexcept
{
	return m_currentDirection;
}


bool TankMovement::CanMove(Direction dir) const
{
	if (dir == Direction::None ||
		dir == Direction::End)
	{
		return false;
	}

	glm::ivec2 nextTile{ m_currentTile + DirectionToGridVector(dir) };

	return m_pGrid->IsPath({ nextTile.x, nextTile.y });
}

glm::ivec2 TankMovement::DirectionToGridVector(Direction dir)
{
	switch (dir)
	{
		case Direction::Up:
		{
			return { 0, -1 };
		}

		case Direction::Down:
		{
			return { 0, 1 };
		}

		case Direction::Left:
		{
			return { -1, 0 };
		}

		case Direction::Right:
		{
			return { 1, 0 };
		}

		default:
		{
			return { 0, 0 };
		}
	}
}


bool TankMovement::IsOppositeDirection(Direction dirA, Direction dirB)
{
	return DirectionToGridVector(dirA) == -DirectionToGridVector(dirB);
}

TankMovement::Direction TankMovement::GetOpposite(TankMovement::Direction dir)
{
	switch (dir)
	{
		case Direction::Up: return Direction::Down;
		case Direction::Down: return Direction::Up;
		case Direction::Left: return Direction::Right;
		case Direction::Right: return Direction::Left;
		default: return Direction::None;
	}
}

glm::ivec2 const& TankMovement::CurrentTile() const noexcept
{
	return m_currentTile;
}

glm::ivec2 const& TankMovement::TargetTile() const noexcept
{
	return m_targetTile;
}

GameGrid& TankMovement::Grid() const noexcept
{
	return *m_pGrid;
}

void TankMovement::RequestMovement(Direction dir)
{
	m_requestedDirection = dir;
	if (IsOppositeDirection(m_currentDirection, dir))
	{
		m_currentDirection = dir;
		std::swap(m_currentTile, m_targetTile);
	}
	m_movementRequested = true;
}

void TankMovement::SetMoveSpeed(float speed)
{
	m_moveSpeed = speed;
}

void TankMovement::Teleport(glm::ivec2 const& gridPos)
{
	m_currentTile = gridPos;
	m_targetTile = gridPos;
	m_currentDirection = Direction::None;
	m_requestedDirection = Direction::None;
	m_movementRequested = false;

	Object()->Transform().SetWorldPosition(m_pGrid->GridToWorld(gridPos));
}


void TankMovement::Awake()
{
	auto worldPos = Object()->Transform().WorldPosition();
	m_currentTile = m_pGrid->WorldToGrid({ worldPos.x, worldPos.y });
	m_targetTile = m_currentTile;
}


void TankMovement::FixedUpdate()
{
	if (m_movementRequested)
	{
		Move(static_cast<float>(mg::DeltaClock::FixedDeltaTime()));

		m_movementRequested = false;
	}
}

TankMovement::TankMovement(mg::GameObject& owner, GameGrid& pGrid)
	: Component(owner)
	, m_pGrid(&pGrid)
{
}

void TankMovement::Move(float elapsedSec)
{
	auto distToMove = m_moveSpeed * elapsedSec;

	auto newPos = Object()->Transform().WorldPosition();

	auto targetPos = m_pGrid->GridToWorld(m_targetTile);

	while (distToMove > 0.f)
	{
		glm::vec2 vecToTarget{ targetPos - newPos };
		auto distToTarget = glm::length(vecToTarget);

		if (distToMove >= distToTarget)
		{
			distToMove -= distToTarget;

			newPos = targetPos;

			m_currentTile = m_targetTile;

			if (CanMove(m_requestedDirection))
			{
				m_currentDirection = m_requestedDirection;
			}

			if (!CanMove(m_currentDirection))
			{
				break;
			}

			m_targetTile = m_currentTile + DirectionToGridVector(m_currentDirection);
			targetPos = m_pGrid->GridToWorld(m_targetTile);
		}
		else
		{
			if (distToTarget != 0)
			{
				newPos += vecToTarget / distToTarget * distToMove;
			}
			distToMove = 0.f;
		}
	}

	Object()->Transform().SetWorldPosition(newPos);
}
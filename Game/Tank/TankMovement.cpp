#include "TankMovement.h"

#include "Grid/GameGrid.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/DeltaClock.h>

#include <glm/geometric.hpp>

void TankMovement::QueueMovement(Direction dir)
{
	m_queuedDirection = dir;
	if (IsOppositeDirection(m_currentDirection, dir))
	{
		m_currentDirection = dir;
		std::swap(m_currentTile, m_targetTile);
	}
	m_shouldMove = true;
}

void TankMovement::Awake()
{
	auto worldPos{ Owner()->Transform().WorldPosition() };
	m_currentTile = m_pGrid->WorldToGrid({ worldPos.x, worldPos.y });
	m_targetTile = m_currentTile;
}

TankMovement::Direction const& TankMovement::MovingDirection() const noexcept
{
	return m_currentDirection;
}

void TankMovement::SetMoveSpeed(float speed)
{
	m_moveSpeed = speed;
}

void TankMovement::FixedUpdate()
{
	if (m_shouldMove)
	{
		MoveToTarget(static_cast<float>(mg::DeltaClock::FixedDeltaTime()));

		m_shouldMove = false;
	}
}

void TankMovement::MoveToTarget(float elapsedSec)
{
	auto distToMove{ m_moveSpeed * elapsedSec };

	auto newPos{ Owner()->Transform().WorldPosition() };

	auto targetPos{ m_pGrid->GridToWorld(m_targetTile) };

	while (distToMove > 0.f)
	{
		glm::vec2 moveTo{ targetPos - newPos };
		auto distToTarget{ glm::length(moveTo) };

		if (distToMove >= distToTarget)
		{
			distToMove -= distToTarget;

			newPos = targetPos;

			m_currentTile = m_targetTile;

			if (CanMove(m_queuedDirection))
			{
				m_currentDirection = m_queuedDirection;
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
				newPos += moveTo / distToTarget * distToMove;
			}
			distToMove = 0.f;
		}
	}

	Owner()->Transform().SetWorldPosition(newPos);
}


bool TankMovement::CanMove(Direction dir) const
{
	if (dir == Direction::None)
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

TankMovement::TankMovement(mg::GameObject& owner, GameGrid& pGrid)
	: Component(owner)
	, m_pGrid(&pGrid)
{
}
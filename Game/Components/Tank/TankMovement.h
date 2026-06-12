#ifndef TANKMOVEMENTCOMPONENT_H
#define TANKMOVEMENTCOMPONENT_H

#include "Minigin/Components/Component.h"

#include <glm/vec2.hpp>

class GameGrid;

class TankMovement final : public mg::Component
{
public:
	enum class Direction
	{
		None,
		Up,
		Down,
		Left,
		Right,
		End
	};


	static glm::ivec2 DirectionToGridVector(Direction dir);
	static bool IsOppositeDirection(Direction dirOne, Direction dirTwo);
	static Direction GetOpposite(Direction dir);

	bool CanMove(Direction dir) const;
	Direction const& CurrentDirection() const noexcept;
	glm::ivec2 const& CurrentTile() const noexcept;
	glm::ivec2 const& TargetTile() const noexcept;
	GameGrid& Grid() const noexcept;


	void RequestMovement(Direction dir);
	void SetMoveSpeed(float speed);

	void Teleport(glm::ivec2 const& gridPos);

	void Awake() override;
	void FixedUpdate() override;

	TankMovement(mg::GameObject& owner, GameGrid& pGrid);

private:

	void Move(float elapsedSec);

	GameGrid* m_pGrid;

	Direction m_currentDirection{ Direction::None };
	Direction m_requestedDirection{ Direction::None };

	glm::ivec2 m_currentTile{};
	glm::ivec2 m_targetTile{};

	float m_moveSpeed{ 100.f };
	bool m_movementRequested{};
};

#endif
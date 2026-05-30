#ifndef TANKMOVEMENTCOMPONENT_H
#define TANKMOVEMENTCOMPONENT_H

#include "Minigin/EngineComponents/Component.h"

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

    bool CanMove(Direction dir) const;
    Direction const& MovingDirection() const noexcept;

    void SetMoveSpeed(float speed);

    void FixedUpdate() override;
    void QueueMovement(Direction dir);

    void Awake() override;

    TankMovement( mg::GameObject& owner, GameGrid& pGrid);

private:

    void MoveToTarget(float elapsedSec);
    
    GameGrid* m_pGrid;

    Direction m_currentDirection{};
    Direction m_queuedDirection{};

    glm::ivec2 m_currentTile{};
    glm::ivec2 m_targetTile{};

    bool m_shouldMove{};
    float m_moveSpeed{100.f};
};

#endif
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
        Up,
        Down,
        Left,
        Right,
        None
    };

    void Update() override;
    void MoveToTarget();
    void QueueDirection(Direction dir);

    TankMovement(  mg::GameObject& owner, GameGrid* pGrid, float moveSpeed );

private:
    bool CanMove(Direction dir) const;
    static glm::ivec2 DirectionToGridVector(Direction dir);
    static bool IsOppositeDirection(Direction dirOne, Direction dirTwo);

    GameGrid* m_pGrid;

    Direction m_currentDirection;
    Direction m_queuedDirection;

    glm::ivec2 m_currentTile;
    glm::ivec2 m_targetTile;

    bool m_inputRecieved;
    float m_moveSpeed;
};

#endif
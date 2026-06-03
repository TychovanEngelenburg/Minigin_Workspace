#ifndef ENEMY_BEHAVIOUR_H
#define ENEMY_BEHAVIOUR_H

#include <Minigin/EngineComponents/Component.h>
#include "Game/Tank/TankMovement.h"
#include <glm/vec2.hpp>
#include <vector>

class GameGrid;

class EnemyBehaviour final : public mg::Component
{
public:
    void Awake() override;
    void Update() override;

    EnemyBehaviour(mg::GameObject& owner, GameGrid& grid);

private:
    void ChooseNextDirection(glm::ivec2 const& gridPos);
    std::vector<TankMovement::Direction> GetValidDirections(glm::ivec2 const& gridPos) const;

    //glm::ivec2 DirToOffset(Direction d) const;
    //Direction Opposite(Direction d) const;

    bool IsAtCenterOfTile() const;
    glm::ivec2 CurrentGridPos() const;

    GameGrid* m_pGrid;
    TankMovement* m_pMovement{};
    TankMovement::Direction m_currentDir{ TankMovement::Direction::None };

    bool m_hasChosenThisTile{ false };

    float m_speed{ 80.f };
};

#endif
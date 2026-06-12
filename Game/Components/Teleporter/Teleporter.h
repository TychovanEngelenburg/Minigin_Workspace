#ifndef TELEPORTER_H
#define TELEPORTER_H

#include <Minigin/Components/Component.h>

#include <glm/vec2.hpp>
#include <vector>
#include <optional>

class GameGrid;
class TankManager;
class TankMovement;
class Teleporter final : public mg::Component
{
public:
    Teleporter(mg::GameObject& owner, GameGrid& grid, TankManager& tankManager);

    void Start() override;
    void OnCollisionEnter(mg::CollisionData const& data) override;

private:
    std::optional<glm::ivec2> FindRandomDestination() const;

    GameGrid* m_pGrid;
    TankManager* m_pTankManager;
};

#endif // !TELEPORTER_H
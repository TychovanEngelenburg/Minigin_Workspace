#ifndef BULLET_POOL_H
#define BULLET_POOL_H

#include <Minigin/EngineComponents/Component.h>

#include <glm/vec2.hpp>
#include <vector>
#include <stack>

class BulletMovement;
class GameGrid;

class BulletPool final : public mg::Component
{
public:


    BulletMovement* SpawnBullet(glm::vec2 const& pos, glm::vec2 const& dir);
    
    void ReturnBullet(BulletMovement* bullet);
    void Awake() override;

    BulletPool(mg::GameObject& owner, GameGrid& grid, size_t initialSize);

private:
    void CreateBullets(size_t count);
    

    GameGrid& m_Grid;
    size_t m_initSize;
    std::stack<BulletMovement*> m_FreeList{};
};

#endif // !BULLET_POOL_H

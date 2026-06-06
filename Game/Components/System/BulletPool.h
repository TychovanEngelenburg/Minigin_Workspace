#ifndef BULLET_POOL_H
#define BULLET_POOL_H

#include <Minigin/Components/Component.h>

#include <stack>

class BulletMovement;
struct BulletConfig;
class GameGrid;

class BulletPool final : public mg::Component
{
public:
    BulletMovement* SpawnBullet(BulletConfig const& config, int killerId = -1);
    
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

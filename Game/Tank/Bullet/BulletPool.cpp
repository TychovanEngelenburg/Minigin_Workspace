#include "BulletPool.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/Scene/Scene.h>
#include "Tank/Bullet/BulletMovement.h"
#include "DamageOnCollision.h"
#include "Game/Grid/GameGrid.h"
#include <Minigin/CollisionSystem/BoxCollider2D.h>
#include <Minigin/EngineComponents/Sprite.h>

BulletMovement* BulletPool::SpawnBullet(glm::vec2 const& pos, glm::vec2 const& dir)
{
    if (m_FreeList.empty())
    {
        return nullptr;
    }

    auto* bullet = std::move(m_FreeList.top());
    m_FreeList.pop();

    bullet->Shoot(pos, dir);

    return bullet;
}


void BulletPool::ReturnBullet(BulletMovement* bullet)
{
    bullet->Owner()->SetActive(false);
    m_FreeList.push(bullet);
}

void BulletPool::Awake()
{
    CreateBullets(m_initSize);
}

BulletPool::BulletPool(mg::GameObject& owner, GameGrid& grid, size_t initialSize)
    : Component(owner)
    , m_Grid(grid)
    , m_initSize(initialSize)
{
}

void BulletPool::CreateBullets(size_t count)
{
    for (size_t i{}; i < count; ++i)
    {
        auto bulletObj = std::make_unique<mg::GameObject>(
            "Bullet_" + std::to_string(i),
            glm::vec3{}
        );

        auto& moveScript = bulletObj->AddComponent<BulletMovement>(m_Grid);
        moveScript.SetPool(this);

        bulletObj->AddComponent<DamageOnCollision>(1);

        auto& sprite =
            bulletObj->AddComponent<mg::Sprite>(
                "T_SpriteSheet_BattleTanks.png",
                mg::SpriteSheet(13, 5));

        sprite.SetSprite({ 1, 2 });

        auto& hitBox = bulletObj->AddComponent<mg::BoxCollider2D>();
        hitBox.SetSize({ 10.f, 8.f });

        bulletObj->SetActive(false);

        m_FreeList.push(&moveScript);

        Owner()->Scene()->Add(std::move(bulletObj));
    }
}
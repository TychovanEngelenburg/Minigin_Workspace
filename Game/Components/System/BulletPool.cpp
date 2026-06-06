#include "BulletPool.h"

#include "Game/Components/Bullet/BulletMovement.h"
#include "Game/Components/DamageOnCollision.h"
#include "Game/Components/System/GameGrid.h"
#include "Game/Config/BulletConfig.h"

#include <Minigin/Scene/Scene.h>
#include <Minigin/Scene/GameObject.h>
#include <Minigin/Collisions/BoxCollider2D.h>
#include <Minigin/Components/Sprite.h>

BulletMovement* BulletPool::SpawnBullet(BulletConfig const& config, int killerId)
{
	if (m_FreeList.empty())
	{
		return nullptr;
	}

	auto bullet = std::move(m_FreeList.top());
	m_FreeList.pop();

	bullet->SetBounces(config.Bounces);
	bullet->SetSpeed(config.Speed);

	bullet->Sprite().SetSprite(config.Sprite.SheetPos);
	bullet->Sprite().SetPivot(config.ColliderSize / 2.f);
	bullet->Collider().SetSize(config.ColliderSize);
	bullet->Collider().SetCenter({ 0.f, 0.f });

	bullet->Collider().CollisionLayer = config.Collisions.Layer;
	bullet->Collider().CollisionMask  = config.Collisions.LayerMask;

	bullet->CollissionDamage().SetKillerId(killerId);
	bullet->CollissionDamage().SetDamageAmount(config.Damage);
	return bullet;
}


void BulletPool::ReturnBullet(BulletMovement* bullet)
{
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
			"Bullet_" + std::to_string(i)
		);

		auto& movementComp = bulletObj->AddComponent<BulletMovement>(m_Grid);
		movementComp.SetPool(this);

		bulletObj->AddComponent<DamageOnCollision>();
		bulletObj->AddComponent<mg::Sprite>(BulletSpriteSheet);
		bulletObj->AddComponent<mg::BoxCollider2D>();

		bulletObj->SetActive(false);

		m_FreeList.push(&movementComp);
		Object()->Scene()->Add(std::move(bulletObj));
	}
}
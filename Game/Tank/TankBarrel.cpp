#include "TankBarrel.h"

#include "Game/Grid/GameGrid.h"
#include "Tank/Bullet/BulletPool.h"

#include <Minigin/Scene/GameObject.h>

void TankBarrel::Shoot()
{
	auto& transform = Owner()->Transform();

	auto angle = glm::radians(transform.WorldRotationZ());

	glm::vec2 direction
	{
		cos(angle),
		sin(angle)
	};

	float barrelLength{ 20.f };

	glm::vec2 spawnPos{ transform.WorldPosition() + direction * barrelLength };

	if (m_pBulletPool)
	{
		m_pBulletPool->SpawnBullet(spawnPos + direction, direction);
	}
}

void TankBarrel::SetBulletPool(BulletPool* pool)
{
	m_pBulletPool = pool;
}

TankBarrel::TankBarrel(mg::GameObject& owner, GameGrid& grid)
	: Component(owner)
	, m_pGrid(&grid)
{
}

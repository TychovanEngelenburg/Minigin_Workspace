#include "Game/Tank/TankBarrel.h"

#include "Game/Config/BulletConfig.h"
#include "Game/Tank/Bullet/BulletMovement.h"
#include "Game/Tank/Bullet/BulletPool.h"
#include "Game/Grid/GameGrid.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/EngineComponents/Sprite.h>
#include <Minigin/CollisionSystem/BoxCollider2D.h>
#include <Minigin/DeltaClock.h>

bool TankBarrel::OnCooldown() const noexcept
{
	return m_currentCooldown > 0;
}

BulletConfig const& TankBarrel::Config() const noexcept
{
	return m_config;
}

void TankBarrel::Shoot()
{
	auto& transform = Owner()->Transform();
	auto angle = glm::radians(transform.WorldRotationZ());


	glm::vec2 forwardDir
	{
		cos(angle),
		sin(angle)
	};

	glm::vec2 spawnPos{ transform.WorldPosition() +  forwardDir * m_barrelLenght};
	spawnPos -= glm::vec2{ m_config.ColliderSize.x * 0.5f, m_config.ColliderSize.y * 0.5f };

	if (m_pBulletPool)
	{
		auto bullet = m_pBulletPool->SpawnBullet(m_config);
		
		if (!bullet)
		{
			return;
		}

		bullet->Owner()->Transform().SetWorldPosition(spawnPos);
		bullet->Activate(spawnPos + forwardDir, forwardDir);
	}

	m_currentCooldown = m_cooldownDuration;
}

void TankBarrel::SetBulletPool(BulletPool* pool)
{
	m_pBulletPool = pool;
}

void TankBarrel::SetBarrelLength(float length)
{
	m_barrelLenght = length;
}
void TankBarrel::SetBulletConfig(BulletConfig const& config)
{
	m_config = config;
}

void TankBarrel::SetCooldown(float duration)
{
	m_cooldownDuration = duration;
}

void TankBarrel::Update()
{
	if (OnCooldown())
	{
		m_currentCooldown -= static_cast<float>(mg::DeltaClock::DeltaTime());;
	}
}

TankBarrel::TankBarrel(mg::GameObject& owner, GameGrid& grid)
	: Component(owner)
	, m_pGrid(&grid)
{
}

#include "TankBarrel.h"

#include "Game/Components/Bullet/BulletMovement.h"
#include "Game/Components/System/BulletPool.h"
#include "Game/Components/System/GameGrid.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/Core/DeltaClock.h>

#include <glm/trigonometric.hpp>

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
	auto& transform = Object()->Transform();
	auto forwardDir = Object()->Transform().Forward();

	glm::vec2 spawnPos{ transform.WorldPosition() +  forwardDir * m_barrelLenght};
	spawnPos -= glm::vec2{ m_config.ColliderSize.x * 0.5f, m_config.ColliderSize.y * 0.5f };

	if (m_pBulletPool)
	{
		auto* bullet = m_pBulletPool->SpawnBullet(m_config, m_playerId);
		
		if (!bullet)
		{
			return;
		}

		bullet->Object()->Transform().SetWorldPosition(spawnPos);
		bullet->Activate(spawnPos + forwardDir, forwardDir);
	}

	m_currentCooldown = m_cooldownDuration;
}

void TankBarrel::SetPlayerId(std::optional<int> playerId)
{
	m_playerId = playerId;
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

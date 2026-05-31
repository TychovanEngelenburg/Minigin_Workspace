#include "TankBarrel.h"

#include "Tank/Bullet/BulletMovement.h"
#include "DamageOnCollision.h"
#include "Game/Grid/GameGrid.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/Scene/Scene.h>
#include <Minigin/EngineComponents/Sprite.h>

#include <memory>

void TankBarrel::Shoot()
{
	if (m_pBullet->Owner()->IsActive())
	{
		return;
	}
	auto& transform = Owner()->Transform();
	auto angle = glm::radians(transform.WorldRotationZ());
	
	glm::vec2 direction{ cos(angle), sin(angle) };
	float barrelLength{ 32.0f };
	glm::vec2 spawnPos = transform.WorldPosition() + direction * barrelLength;

	m_pBullet->Shoot(spawnPos + direction, direction);
}

void TankBarrel::Awake()
{
	SpawnBullet();
}

TankBarrel::TankBarrel(mg::GameObject& owner, GameGrid& grid)
	: Component(owner)
	, m_pGrid(&grid)
{
}


void TankBarrel::SpawnBullet()
{

	auto bullet = std::make_unique<mg::GameObject>("PlayerBullet_", glm::vec3(36.f, 146.f, 0.f));

	auto& moveScript = bullet->AddComponent<BulletMovement>(*m_pGrid);
	bullet->AddComponent<DamageOnCollision>(1);

	auto& sprite{ bullet->AddComponent<mg::Sprite>("T_SpriteSheet_BattleTanks.png", mg::SpriteSheet(13, 5)) };
	sprite.SetSprite({ 1, 2 });

	auto& hitBox{ bullet->AddComponent<mg::BoxCollider2D>() };
	hitBox.SetSize({ 10.f, 8.f });



	m_pBullet = &moveScript;
	Owner()->Scene()->Add(std::move(bullet));
}

#include "TankBarrel.h"

#include "Tank/Bullet/BulletMovement.h"
#include "DamageOnCollision.h"


#include <Minigin/Scene/GameObject.h>
#include <Minigin/Scene/Scene.h>
#include <Minigin/EngineComponents/Sprite.h>

#include <memory>

void TankBarrel::Shoot()
{

	if (m_pBullet->IsActive())
	{
		return;
	}
	auto& transform{ Owner()->Transform() };
	m_pBullet->Shoot(transform.WorldPosition(), glm::vec2( sin(transform.WorldRotationZ()), cos(transform.WorldRotationZ())));
}

void TankBarrel::Awake()
{
}

TankBarrel::TankBarrel(mg::GameObject& owner)
	: Component(owner)
	//, m_pGrid{pGrid}
{
}


void TankBarrel::SpawnBullet()
{

	//auto bullet = std::make_unique<mg::GameObject>("PlayerBullet_", glm::vec3(36.f, 146.f, 0.f));
	//{
	//	//bullet->AddComponent<BulletMovement>(grid->GetComponent<GameGrid>());
	//	bullet->AddComponent<DamageOnCollision>(1);

	//	auto& sprite{ bullet->AddComponent<mg::Sprite>("T_SpriteSheet_Tron.png", mg::SpriteSheet(13, 5)) };
	//	sprite.SetSprite(6, 0);

	//	auto& hitBox{ bullet->AddComponent<mg::BoxCollider2D>() };
	//	hitBox.SetSize({ 10.f, 8.f });
	//}





	////if (m_pBullet)
	////{
	////	Owner()->Scene()->Remove(*m_pBullet->Owner());
	////}

	////auto bullet = std::make_unique<mg::GameObject>("PlayerBullet_" + Owner()->Name(), glm::vec3(0.f, 0.f, 0.f));
	////
	////auto& bulletMovement{ bullet->AddComponent<BulletMovement>() };
	////m_pBullet = &bulletMovement;

	////auto& sprite{ bullet->AddComponent<mg::Sprite>("T_SpriteSheet_Tron.png", mg::SpriteSheet(13, 5)) };
	////sprite.SetSprite(6, 0);

	////bullet->SetActive(false);
	////Owner()->Scene()->Add(std::move(bullet));
}

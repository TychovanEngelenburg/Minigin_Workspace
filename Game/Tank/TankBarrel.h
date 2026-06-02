#ifndef TANK_BARREL_H
#define TANK_BARREL_H

#include <Minigin/EngineComponents/Component.h>
#include "Game/Tank/Bullet/BulletConfig.h"

class BulletPool;
class GameGrid;

class TankBarrel final : public mg::Component
{
public:
	void Shoot();
	void SetBulletPool(BulletPool* pool);
	void SetBarrelLength(float length);
	void SetBulletConfig(BulletConfig const& config);

	TankBarrel(mg::GameObject& owner, GameGrid& grid);

private:
	BulletPool* m_pBulletPool{};
	GameGrid* m_pGrid{};
	BulletConfig m_config;
	float m_barrelLenght{ 0.f };
};


#endif // !TANK_BARREL_H

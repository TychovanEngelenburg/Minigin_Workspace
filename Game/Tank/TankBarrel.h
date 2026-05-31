#ifndef TANK_BARREL_H
#define TANK_BARREL_H

#include <Minigin/EngineComponents/Component.h>

class BulletPool;
class GameGrid;

class TankBarrel final : public mg::Component
{
public:
	void Shoot();
	void SetBulletPool(BulletPool* pool);

	TankBarrel(mg::GameObject& owner, GameGrid& grid);

private:
	BulletPool* m_pBulletPool{};
	GameGrid* m_pGrid{};
};


#endif // !TANK_BARREL_H

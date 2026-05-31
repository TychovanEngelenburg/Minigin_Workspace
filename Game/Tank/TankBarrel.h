#ifndef TANK_BARREL_H
#define TANK_BARREL_H

#include <Minigin/EngineComponents/Component.h>

class BulletMovement;
class GameGrid;

class TankBarrel final : public mg::Component
{
public:
	void Shoot();
	void Awake() override;

	TankBarrel(mg::GameObject& owner, GameGrid& grid);

private:
	void SpawnBullet();

	BulletMovement* m_pBullet{};
	GameGrid* m_pGrid{};
};


#endif // !TANK_BARREL_H

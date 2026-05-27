#ifndef TANK_BARREL_H
#define TANK_BARREL_H

#include <Minigin/EngineComponents/Component.h>

#include <filesystem>

class BulletMovement;
class TankBarrel final : public mg::Component
{
public:
	void Shoot();
	void RotateClockwise();
	void RotateCounterClock();

	TankBarrel(mg::GameObject& owner, std::filesystem::path spritePath);

private:
	void SpawnBullet();

	std::filesystem::path m_spritePath;
	BulletMovement* m_pBullet{};
};


#endif // !TANK_BARREL_H

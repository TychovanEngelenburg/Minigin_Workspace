#ifndef TANK_BARREL_H
#define TANK_BARREL_H

#include <Minigin/Components/Component.h>
#include "Game/Config/BulletConfig.h"

class BulletPool;
class GameGrid;

class TankBarrel final : public mg::Component
{
public:
	bool OnCooldown() const noexcept;
	BulletConfig const& Config() const noexcept;

	void Shoot();

	void SetBulletPool(BulletPool* pool);
	void SetBarrelLength(float length);
	void SetBulletConfig(BulletConfig const& config);

	void SetCooldown(float duration);

	void Update() override;

	TankBarrel(mg::GameObject& owner, GameGrid& grid);

private:
	BulletPool* m_pBulletPool{};
	GameGrid* m_pGrid{};
	BulletConfig m_config;

	float m_barrelLenght{ 0.f };
	float m_cooldownDuration{};
	float m_currentCooldown{};
};


#endif // !TANK_BARREL_H

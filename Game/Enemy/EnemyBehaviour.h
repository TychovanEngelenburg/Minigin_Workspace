#ifndef ENEMY_MOVEMENT_H
#define ENEMY_MOVEMENT_H
#include "Tank/TankMovement.h"

#include <Minigin/EngineComponents/Component.h>

class EnemyBehaviour final : public mg::Component
{
public:
	void Awake() override;
	void Update() override;

	EnemyBehaviour(mg::GameObject& owner);

private:
	
	TankMovement::Direction GetRandomDirection();

	double m_directionTimer{};
	TankMovement* m_pMovement{};
	TankMovement::Direction m_currentDirection{};
};
#endif // !ENEMY_MOVEMENT_H

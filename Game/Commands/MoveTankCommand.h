#ifndef MOVETANKCOMMAND_H
#define MOVETANKCOMMAND_H
#include "Minigin/InputHandling/GameObjectCommand.h" 
#include "Tank/TankMovement.h"

namespace mg
{
	class GameObject;
}

class MoveTankCommand final : public mg::GameObjectCommand
{
public:
	void Execute() override;

	MoveTankCommand(mg::GameObject* object, TankMovement::Direction moveDir);

	~MoveTankCommand() override = default;
	MoveTankCommand(MoveTankCommand const& other) = delete;
	MoveTankCommand(MoveTankCommand&& other) = delete;
	MoveTankCommand& operator=(MoveTankCommand const& other) = delete;
	MoveTankCommand& operator=(MoveTankCommand&& other) = delete;

private:
	TankMovement::Direction m_movementDirection;
	TankMovement* m_pMovementComp;
};
#endif // !MOVEPLAYERCOMMAND_H
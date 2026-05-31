#ifndef MOVETANKCOMMAND_H
#define MOVETANKCOMMAND_H

#include "Tank/TankMovement.h"

#include <Minigin/InputHandling/GameObjectCommand.h>

namespace mg
{
	class GameObject;
}

class MoveTankCommand final : public mg::GameObjectCommand
{
public:
	void Execute() override;

	MoveTankCommand(mg::GameObject& object, TankMovement::Direction moveDir);

private:
	TankMovement::Direction m_movementDirection;
	TankMovement* m_pMovementComp;
};
#endif // !MOVEPLAYERCOMMAND_H
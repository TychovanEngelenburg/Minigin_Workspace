#include "Commands/MoveTankCommand.h"
#include "Minigin/GameObject.h"

void MoveTankCommand::Execute()
{ 
	m_pMovementComp->QueueDirection(m_movementDirection);
}

MoveTankCommand::MoveTankCommand(mg::GameObject* object, TankMovement::Direction moveDir)
	: GameObjectCommand(object)
	, m_movementDirection{ moveDir }
	, m_pMovementComp{ object->GetComponent<TankMovement>() }
{
}

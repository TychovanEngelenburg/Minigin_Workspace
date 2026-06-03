#include "Commands/MoveTankCommand.h"

#include <Minigin/Scene/GameObject.h>

void MoveTankCommand::Execute()
{ 
	m_pMovementComp->RequestMovement(m_movementDirection);
}

MoveTankCommand::MoveTankCommand(mg::GameObject& object, TankMovement::Direction moveDir)
	: GameObjectCommand(object)
	, m_movementDirection( moveDir )
	, m_pMovementComp( object.GetComponent<TankMovement>() )
{
}

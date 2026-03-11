#include "MovePlayerCommand.h"
#include "InputHandling/GameObjectCommand.h" 
#include "GameObject.h"
#include "PlayerMovementComp.h"
#include <iostream>

void MovePlayerCommand::Execute()
{
	m_movementScript->Move(m_movementDirection);
}

MovePlayerCommand::MovePlayerCommand(dae::GameObject* object, glm::vec2 const& moveDir)
	: GameObjectCommand(object)
	, m_movementDirection{moveDir}
	, m_movementScript{object->GetComponent<PlayerMovementComp>()}
{
}

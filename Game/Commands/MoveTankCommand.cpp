#include "Commands/MoveTankCommand.h"
#include "GameObject.h"
#include "DeltaClock.h"
#include <glm/vec2.hpp>

void MoveTankCommand::Execute()
{ 
	m_object->Transform().Translate( m_movementDirection * m_movementSpeed * static_cast<float>(mg::DeltaClock::DeltaTime()) );
}

MoveTankCommand::MoveTankCommand(mg::GameObject* object, glm::vec2 const& moveDir, float moveSpeed)
	: GameObjectCommand(object)
	, m_movementDirection{ glm::normalize(moveDir).x,  glm::normalize(moveDir).y, 0.f }
	, m_movementSpeed{ moveSpeed }
{
}

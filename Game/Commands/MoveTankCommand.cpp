#include "Commands/MoveTankCommand.h"
#include "GameObject.h"
#include "DeltaClock.h"
#include <glm/vec2.hpp>

void MoveTankCommand::Execute()
{ 
	m_object->GetTransform().Translate(
		m_movementDirection.x * m_movementSpeed * static_cast<float>(mg::DeltaClock::GetDeltaTime()),
		m_movementDirection.y * m_movementSpeed * static_cast<float>(mg::DeltaClock::GetDeltaTime())
	);

}

MoveTankCommand::MoveTankCommand(mg::GameObject* object, glm::vec2 const& moveDir, float moveSpeed)
	: GameObjectCommand(object)
	, m_movementDirection{ glm::normalize(moveDir) }
	, m_movementSpeed{ moveSpeed }
{
}

#include "InputHandling/GameObjectCommand.h"
#include "GameObject.h"

dae::GameObject* dae::GameObjectCommand::GetGameObject() const
{
	return m_object;
}

dae::GameObjectCommand::GameObjectCommand(GameObject* object)
	: m_object{object}
{
}

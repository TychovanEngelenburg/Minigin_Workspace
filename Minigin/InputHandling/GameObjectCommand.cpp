#include "InputHandling/GameObjectCommand.h"
#include "GameObject.h"

mg::GameObject* mg::GameObjectCommand::GetGameObject() const
{
	return m_object;
}

mg::GameObjectCommand::GameObjectCommand(GameObject* object)
	: m_object{object}
{
}

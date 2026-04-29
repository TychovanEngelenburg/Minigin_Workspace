#include "Minigin/InputHandling/GameObjectCommand.h"
#include "Minigin/GameObject.h"

mg::GameObject* mg::GameObjectCommand::GetGameObject() const
{
	return m_object;
}

mg::GameObjectCommand::GameObjectCommand(GameObject* object)
	: m_object{object}
{
}

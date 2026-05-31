#ifndef GAMEOBJECTCOMMAND_H
#define GAMEOBJECTCOMMAND_H
#include "Minigin/InputHandling/ICommand.h"

namespace mg
{

	class GameObject;
	class GameObjectCommand : public ICommand
	{
	public:
		GameObjectCommand(GameObject& object);

	protected:
		GameObject* GetGameObject() const;

	private:
		GameObject* m_pObject;

	};
}
#endif // !GAMEOBJECTCOMMAND_H

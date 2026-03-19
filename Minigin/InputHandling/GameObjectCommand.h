#ifndef GAMEOBJECTCOMMAND_H
#define GAMEOBJECTCOMMAND_H
#include "InputHandling/ICommand.h"

namespace mg
{

	class GameObject;
	class GameObjectCommand : public ICommand
	{
	public:
		GameObject* m_object;

		GameObjectCommand(GameObject* object);

		virtual ~GameObjectCommand() = default;
		GameObjectCommand(GameObjectCommand const& other) = delete;
		GameObjectCommand(GameObjectCommand&& other) = delete;
		GameObjectCommand& operator=(GameObjectCommand const& other) = delete;
		GameObjectCommand& operator=(GameObjectCommand&& other) = delete;

	protected:
		GameObject* GetGameObject() const;
	};
}
#endif // !GAMEOBJECTCOMMAND_H

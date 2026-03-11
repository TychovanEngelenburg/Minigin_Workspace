#ifndef MOVEPLAYERCOMMAND_H
#define MOVEPLAYERCOMMAND_H
#include "glm/glm.hpp"
#include "InputHandling/GameObjectCommand.h" 

namespace dae
{
	class GameObject;
}

#include "PlayerMovementComp.h"
class MovePlayerCommand final : public dae::GameObjectCommand
{
public:
	void Execute() override;

	MovePlayerCommand(dae::GameObject* object, glm::vec2 const& moveDir);

	~MovePlayerCommand() override = default;
	MovePlayerCommand(MovePlayerCommand const& other) = delete;
	MovePlayerCommand(MovePlayerCommand&& other) = delete;
	MovePlayerCommand& operator=(MovePlayerCommand const& other) = delete;
	MovePlayerCommand& operator=(MovePlayerCommand&& other) = delete;
private:
	glm::vec2 m_movementDirection;
	PlayerMovementComp* m_movementScript;
};
#endif // !MOVEPLAYERCOMMAND_H
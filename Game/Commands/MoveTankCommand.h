#ifndef MOVETANKCOMMAND_H
#define MOVETANKCOMMAND_H
#include "glm/glm.hpp"
#include "InputHandling/GameObjectCommand.h" 

namespace dae
{
	class GameObject;
}

class MoveTankCommand final : public dae::GameObjectCommand
{
public:
	void Execute() override;

	MoveTankCommand(dae::GameObject* object, glm::vec2 const& moveDir, float moveSpeed);

	~MoveTankCommand() override = default;
	MoveTankCommand(MoveTankCommand const& other) = delete;
	MoveTankCommand(MoveTankCommand&& other) = delete;
	MoveTankCommand& operator=(MoveTankCommand const& other) = delete;
	MoveTankCommand& operator=(MoveTankCommand&& other) = delete;
private:
	glm::vec2 m_movementDirection;
	float m_movementSpeed;
};
#endif // !MOVEPLAYERCOMMAND_H
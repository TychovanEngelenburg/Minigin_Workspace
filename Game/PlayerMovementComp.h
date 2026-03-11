#ifndef PLAYERMOVEMENTCOMP_H
#define PLAYERMOVEMENTCOMP_H

#include "EngineComponents/Component.h"
#include "glm/glm.hpp"

namespace dae
{
	class GameObject;
};
class PlayerMovementComp : public dae::Component
{
public:
	void FixedUpdate() override;

	void Move(glm::vec2 const& m_direction);

	PlayerMovementComp(dae::GameObject& owner, float movementSpeed);


private:
	float m_movementSpeed;
	glm::vec2 m_moveDir; // quick and dirty way for movement, replace later.
};
#endif // !PLAYERMOVEMENTCOMP_H

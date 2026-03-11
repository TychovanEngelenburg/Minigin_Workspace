#include "PlayerMovementComp.h"
#include "GameObject.h"
#include "DeltaClock.h"
#include <glm/vec2.hpp>

void PlayerMovementComp::Move(glm::vec2 const& m_direction)
{
	m_moveDir += m_direction;
}

void PlayerMovementComp::FixedUpdate()
{
	if (glm::length(m_moveDir) > 0)
	{
		glm::vec2 velocity
		{
			glm::normalize(m_moveDir).x * m_movementSpeed* dae::DeltaClock::GetFixedDeltaTime(),
			glm::normalize(m_moveDir).y * m_movementSpeed* dae::DeltaClock::GetFixedDeltaTime()
		};

		GetOwner()->GetTransform().Translate(velocity.x, velocity.y);
		m_moveDir.x = 0;
		m_moveDir.y = 0;
	}

}

PlayerMovementComp::PlayerMovementComp(dae::GameObject& owner, float movementSpeed)
	: Component(owner)
	, m_movementSpeed{ movementSpeed }
	, m_moveDir{}
{

}


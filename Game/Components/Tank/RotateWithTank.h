#ifndef ROTATE_WITH_MOVEMENT_H
#define ROTATE_WITH_MOVEMENT_H

#include <Minigin/Components/Component.h>
#include "TankMovement.h"
#include "glm/vec2.hpp"

namespace mg
{
	class Sprite;
};


class RotateWithTank final : public mg::Component
{
public:
	void SetTank(TankMovement* tank);
	void Update() override;

	explicit RotateWithTank(mg::GameObject& owner);

private:
	void CheckDirection();
	void SetRotation();

	TankMovement* m_pMovement{};
	TankMovement::Direction m_currentDirection{};
};

#endif // !ROTATE_WITH_MOVEMENT_H
#ifndef TANK_VISUALS_H
#define TANK_VISUALS_H

#include <Minigin/EngineComponents/Component.h>
#include "TankMovement.h"
#include "glm/vec2.hpp"

namespace mg
{
	class Sprite;
};


class TankVisuals final : public mg::Component
{
public:
	void Awake() override;
	void Update() override;

	TankVisuals(mg::GameObject& owner, glm::ivec2 const& sprite);

private:
	void SetSprite();

	mg::Sprite* m_pSprite{};
	TankMovement* m_pMovement{};
	TankMovement::Direction m_currentDirection{};
	glm::ivec2 spritePos;
};

#endif // !TANK_VISUALS_H
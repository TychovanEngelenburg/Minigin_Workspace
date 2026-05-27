#ifndef TANK_VISUALS_H
#define TANK_VISUALS_H

#include <Minigin/EngineComponents/Component.h>

namespace mg
{
	class Sprite;
};

class TankVisuals final : public mg::Component
{
public:
	TankVisuals(mg::GameObject& owner);

private:
	mg::Sprite* m_pSprite;
};

#endif // !TANK_VISUALS_H
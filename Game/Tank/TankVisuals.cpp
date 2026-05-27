#include "TankVisuals.h"

#include <Minigin/EngineComponents/Sprite.h>
#include <Minigin/Scene/GameObject.h>

TankVisuals::TankVisuals(mg::GameObject& owner)
	: Component(owner)
	, m_pSprite {Owner()->GetComponent<mg::Sprite>()}
{

}

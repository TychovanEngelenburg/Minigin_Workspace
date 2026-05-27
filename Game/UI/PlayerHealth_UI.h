#ifndef PLAYERHEALTH_UI_H
#define PLAYERHEALTH_UI_H

#include "IEventListener.h"
#include "Helper/GameEvents.h"

#include <Minigin/EngineComponents/Component.h>

namespace mg
{
	class TextComponent;
	class GameObject;
};

class PlayerHealth_UI final : public mg::Component, public mg::IEventListener<PlayerLivesChangedEvent>
{
public:

	void OnNotify(PlayerLivesChangedEvent const& eventData) override;

	explicit PlayerHealth_UI(mg::GameObject& owner);

private:
	mg::TextComponent* m_pTextComp;
};
#endif // !PLAYERHEALTH_UI_H
				 
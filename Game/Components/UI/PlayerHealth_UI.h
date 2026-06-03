#ifndef PLAYERHEALTH_UI_H
#define PLAYERHEALTH_UI_H

#include "Events/GameEvents.h"

#include <Minigin/Events/IEventListener.h>
#include <Minigin/Components/Component.h>

namespace mg
{
	class TextComponent;
	class GameObject;
};

class PlayerHealth_UI final : public mg::Component, public mg::IEventListener<PlayerDeath>
{
public:

	void OnNotify(PlayerDeath const& eventData) override;

	explicit PlayerHealth_UI(mg::GameObject& owner);

	virtual ~PlayerHealth_UI() = default;
	PlayerHealth_UI(PlayerHealth_UI const& other) = delete;
	PlayerHealth_UI(PlayerHealth_UI&& other) = delete;
	PlayerHealth_UI& operator=(PlayerHealth_UI const& other) = delete;
	PlayerHealth_UI& operator=(PlayerHealth_UI&& other) = delete;
private:
	mg::TextComponent* m_pTextComp;
};
#endif // !PLAYERHEALTH_UI_H
				 
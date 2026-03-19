#ifndef PLAYERHEALTH_UI_H
#define PLAYERHEALTH_UI_H
#include "EngineComponents/Component.h"
#include "IEventListener.h"
#include "GameEvents.h"
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

	~PlayerHealth_UI() override = default;
	PlayerHealth_UI(PlayerHealth_UI const& other) = delete;
	PlayerHealth_UI(PlayerHealth_UI&& other) = delete;
	PlayerHealth_UI& operator=(PlayerHealth_UI const& other) = delete;
	PlayerHealth_UI& operator=(PlayerHealth_UI&& other) = delete;

private:
	mg::TextComponent* m_pTextComp;
};
#endif // !PLAYERHEALTH_UI_H
				 
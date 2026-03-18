#ifndef PLAYERHEALTH_UI_H
#define PLAYERHEALTH_UI_H
#include "EngineComponents/Component.h"
#include "EventListener.h"
#include "GameEvents.h"
namespace dae
{
	class TextComponent;
	class GameObject;
};

class PlayerHealth_UI : public dae::Component, public dae::EventListener<PlayerLivesChangedEvent>
{
public:

	void OnNotify(PlayerLivesChangedEvent const& eventData) override;

	PlayerHealth_UI(dae::GameObject& owner);

	~PlayerHealth_UI() override = default;
	PlayerHealth_UI(PlayerHealth_UI const& other) = delete;
	PlayerHealth_UI(PlayerHealth_UI&& other) = delete;
	PlayerHealth_UI& operator=(PlayerHealth_UI const& other) = delete;
	PlayerHealth_UI& operator=(PlayerHealth_UI&& other) = delete;

private:
	dae::TextComponent* m_textComp;
};
#endif // !PLAYERHEALTH_UI_H
				 
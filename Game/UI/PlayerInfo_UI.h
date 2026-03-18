#ifndef PLAYERINFO_UI_H
#define PLAYERINFO_UI_H
#include "EngineComponents/Component.h"
#include "EventListener.h"
#include "GameEvents.h"
namespace dae
{
	class TextComponent;
	class GameObject;
};

class PlayerInfo_UI : public dae::Component, public dae::EventListener<PlayerLivesChangedEvent>
{
public:

	void OnNotify(PlayerLivesChangedEvent const& eventData) override;

	PlayerInfo_UI(dae::GameObject& owner);

	~PlayerInfo_UI() override = default;
	PlayerInfo_UI(PlayerInfo_UI const& other) = delete;
	PlayerInfo_UI(PlayerInfo_UI&& other) = delete;
	PlayerInfo_UI& operator=(PlayerInfo_UI const& other) = delete;
	PlayerInfo_UI& operator=(PlayerInfo_UI&& other) = delete;

private:
	dae::TextComponent* m_textComp;
};
#endif // !PLAYERINFO_UI_H

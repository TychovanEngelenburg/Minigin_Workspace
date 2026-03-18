#include "UI/PlayerInfo_UI.h"
#include "EngineComponents/Component.h"
#include "EngineComponents/TextComponent.h"
#include "GameObject.h"
#include <cassert>
#include <string>

void PlayerInfo_UI::OnNotify(PlayerLivesChangedEvent const& eventData)
{
	if (!m_textComp)
	{
		m_textComp = GetOwner()->GetComponent<dae::TextComponent>();
		assert(m_textComp);
	}

	m_textComp->SetText(std::to_string(eventData.livesLeft));
}

PlayerInfo_UI::PlayerInfo_UI(dae::GameObject& owner)
	: Component(owner)
	, m_textComp{}
{
}

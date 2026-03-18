#include "UI/PlayerHealth_UI.h"
#include "EngineComponents/Component.h"
#include "EngineComponents/TextComponent.h"
#include "GameObject.h"
#include <cassert>
#include <string>

void PlayerHealth_UI::OnNotify(PlayerLivesChangedEvent const& eventData)
{
	if (!m_textComp)
	{
		m_textComp = GetOwner()->GetComponent<dae::TextComponent>();
		assert(m_textComp);
	}

	m_textComp->SetText("# lives:" + std::to_string(eventData.livesLeft));
}

PlayerHealth_UI::PlayerHealth_UI(dae::GameObject& owner)
	: Component(owner)
	, m_textComp{}
{
}

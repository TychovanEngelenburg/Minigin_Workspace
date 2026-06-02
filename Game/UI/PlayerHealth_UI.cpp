#include "UI/PlayerHealth_UI.h"

#include <Minigin/EngineComponents/Component.h>
#include <Minigin/EngineComponents/TextComponent.h>
#include <Minigin/Scene/GameObject.h>

#include <cassert>
#include <string>

void PlayerHealth_UI::OnNotify(PlayerDeath const& eventData)
{
	if (!m_pTextComp)
	{
		m_pTextComp = Owner()->GetComponent<mg::TextComponent>();
		assert(m_pTextComp);
	}

	m_pTextComp->SetText("# lives:" + std::to_string(eventData.playerId));
}

PlayerHealth_UI::PlayerHealth_UI(mg::GameObject& owner)
	: Component(owner)
	, m_pTextComp{}
{
}

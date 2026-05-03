#include "UI/PlayerHealth_UI.h"
#include "EngineComponents/Component.h"
#include "EngineComponents/TextComponent.h"
#include "Minigin/GameObject.h"
#include <cassert>
#include <string>

void PlayerHealth_UI::OnNotify(PlayerLivesChangedEvent const& eventData)
{
	if (!m_pTextComp)
	{
		m_pTextComp = Owner()->GetComponent<mg::TextComponent>();
		assert(m_pTextComp);
	}

	m_pTextComp->SetText("# lives:" + std::to_string(eventData.livesLeft));
}

PlayerHealth_UI::PlayerHealth_UI(mg::GameObject& owner)
	: Component(owner)
	, m_pTextComp{}
{
}

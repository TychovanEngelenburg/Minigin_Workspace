#include "PlayerHealth_UI.h"

#include <Minigin/Components/Component.h>
#include <Minigin/Components/TextComponent.h>
#include <Minigin/Scene/GameObject.h>

#include <cassert>
#include <string>

void PlayerHealth_UI::OnNotify(TankDeathEvent const&)
{
	if (!m_pTextComp)
	{
		m_pTextComp = Object()->GetComponent<mg::TextComponent>();
		assert(m_pTextComp);
	}

	//m_pTextComp->SetText("# lives:" + std::to_string(eventData.playerId));
}

PlayerHealth_UI::PlayerHealth_UI(mg::GameObject& owner)
	: Component(owner)
{
}

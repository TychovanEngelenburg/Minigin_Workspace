#include "PlayerHealth_UI.h"

#include <Minigin/Components/Component.h>
#include <Minigin/Components/TextComponent.h>
#include <Minigin/Scene/GameObject.h>

#include <cassert>
#include <string>

void PlayerHealth_UI::OnNotify(TankDeathEvent const& eventData)
{
	if (!m_pTextComp)
	{
		m_pTextComp = Object()->GetComponent<mg::TextComponent>();
		assert(m_pTextComp);
	}
	if (true)
	{

	}
	m_pTextComp->SetText("# lives:" ));
}

PlayerHealth_UI::PlayerHealth_UI(mg::GameObject& owner)
	: Component(owner)
{
}

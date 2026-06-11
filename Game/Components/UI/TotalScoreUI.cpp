#include "TotalScoreUI.h"

#include "Game/Core/GameContext.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/Components/TextComponent.h>

void TotalScoreUI::UpdateText()
{
	m_pTextComp->SetText(std::to_string(GameContext::Instance().TotalScore()));
}

TotalScoreUI::TotalScoreUI(mg::GameObject& owner)
	: Component(owner)
{
}

void TotalScoreUI::Awake()
{
	m_pTextComp = Object()->GetComponent<mg::TextComponent>();
	assert(m_pTextComp && "TotalScoreUI requires a TextComponent on it's GameObject!");

	UpdateText();
}

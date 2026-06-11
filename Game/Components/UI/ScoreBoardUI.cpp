#include "ScoreBoardUI.h"

#include "Game/Core/GameContext.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/Components/TextComponent.h>

void ScoreBoardUI::UpdateText()
{
    auto const& scores = GameContext::Instance().ScoreManager().GetScoreboard(GameContext::Instance().Mode());

    std::string output{};

    int rank = 1;
    for (auto const& entry : scores)
    {
        output += std::to_string(rank++) + ". " + entry.name.data() + " - " + std::to_string(entry.score)  + "\n";
    }

    if (m_pTextComp)
    {

        m_pTextComp->SetText(output);
    }
}

void ScoreBoardUI::Awake()

{
	m_pTextComp = Object()->GetComponent<mg::TextComponent>();
	assert(m_pTextComp && "TotalScoreUI requires a TextComponent on it's GameObject!");

	UpdateText();
}

ScoreBoardUI::ScoreBoardUI(mg::GameObject& owner)
    : Component(owner)
{}

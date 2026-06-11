#include "PlayerUI.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/Components/TextComponent.h>
#include <iostream>

void PlayerUI::SetPlayerId(int id)
{
	m_playerId = id;
}

void PlayerUI::SetScoreComp(mg::TextComponent* pTextComp)
{
	m_pScoreText = pTextComp;
}

void PlayerUI::SetSLivesComp(mg::TextComponent* pTextComp)
{
	m_pLivesText = pTextComp;
}

void PlayerUI::OnNotify(ScoreChangedEvent const& eventData)
{
	if (eventData.PlayerId != m_playerId || !m_pScoreText)
	{
		return;
	}

	m_pScoreText->SetText(std::to_string(eventData.Score));
	std::cout << "Player " << m_playerId << "'s score is now: " << eventData.Score << "\n";
}

void PlayerUI::OnNotify(LivesChangedEvent const& eventData)
{
	if (eventData.PlayerId != m_playerId || !m_pLivesText)
	{
		return;
	}

	if (eventData.Lives <= 0)
	{
		m_pLivesText->SetText(m_noLivesText);
		return;
	}
	m_pLivesText->SetText(std::to_string(eventData.Lives  - 1));
	std::cout << "Player " << m_playerId << "'s lives are now: " << eventData.Lives << "\n";
}

PlayerUI::PlayerUI(mg::GameObject& owner)
	: Component(owner)
{
}

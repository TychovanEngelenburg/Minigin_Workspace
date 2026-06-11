#include "GameModeUI.h"

#include "Game/Core/GameContext.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/Components/TextComponent.h>

#include <cassert>

void GameModeUI::OnNotify(GameModeChangedEvent const& event)
{
	UpdateText(event.Mode);
}

void GameModeUI::Start()
{
	GameContext::Instance().OnGameModeChanged.AddListener(this);

	UpdateText(GameContext::Instance().Mode());
}

void GameModeUI::Awake()
{
	m_pTextComp = Object()->GetComponent<mg::TextComponent>();
	assert(m_pTextComp && "TotalScoreUI requires a TextComponent on it's GameObject!");
}

GameModeUI::GameModeUI(mg::GameObject& owner)
	: Component(owner)
{}

GameModeUI::~GameModeUI()
{
	GameContext::Instance().OnGameModeChanged.RemoveListener(this);
}

void GameModeUI::UpdateText(GameMode mode)
{

	std::string text{ "Current mode: " };
	switch (mode)
	{
		case GameMode::Singleplayer:
		{
			text += "Singleplayer";
			break;
		}

		case GameMode::Coop:
		{
			text += "Coop";
			break;
		}

		case GameMode::Versus:
		{
			text += "Versus";
			break;
		}

		default:
		{
			text += "Undefined!";
			break;
		}
	}
	m_pTextComp->SetText(text);

}
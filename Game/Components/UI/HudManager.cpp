#include "HudManager.h"

#include "Game/Components/UI/PlayerUI.h"
#include "Game/Core/GameContext.h"
#include "Game/Config/FileConfig.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/Scene/Scene.h>
#include <Minigin/Components/TextComponent.h>
#include <Minigin/Rendering/SDLRenderer.h>
void HUDManager::Start()
{
	auto& context = GameContext::Instance();

	for (auto const& player : context.Players())
	{
		if (player.AsEnemy)
		{
			continue;
		}

		auto hudObj = std::make_unique<mg::GameObject>("PlayerUI_P" + std::to_string(player.PlayerId));
		auto& ui = hudObj->AddComponent<PlayerUI>();
		ui.SetPlayerId(player.PlayerId);

		// This could be handled in this class instead. Or with a "OnTankDeath" but this is a better demonstration.
		if (player.Lives > 0)
		{
			context.OnScoreChanged.AddListener(&ui);
			context.OnLivesChanged.AddListener(&ui);
		}

		auto livesTextObj = std::make_unique<mg::GameObject>("LivesUI_P" + std::to_string(player.PlayerId));
		auto& livesText = livesTextObj->AddComponent<mg::TextComponent>(Files::JoystixFont, m_fontSize);
		ui.SetSLivesComp(&livesText);
		livesTextObj->Transform().SetParent(&this->Object()->Transform());
		livesTextObj->Transform().SetLocalPosition({ mg::Renderer::Instance().WindowSize().x / context.Players().size() * player.PlayerId, 0.f });

		auto scoreTextObj = std::make_unique<mg::GameObject>("ScoreUI_P" + std::to_string(player.PlayerId));
		auto& scoreText = scoreTextObj->AddComponent<mg::TextComponent>(Files::JoystixFont, m_fontSize);
		ui.SetScoreComp(&scoreText);
		scoreTextObj->Transform().SetParent(&livesTextObj->Transform());
		scoreTextObj->Transform().Translate({ 0, 40 });

		m_pPlayerInfoElements.push_back(&ui);

		ui.OnNotify(LivesChangedEvent(player.PlayerId, player.Lives));
		ui.OnNotify(ScoreChangedEvent(player.PlayerId, player.Score));


		Object()->Scene()->Add(std::move(hudObj));
		Object()->Scene()->Add(std::move(livesTextObj));
		Object()->Scene()->Add(std::move(scoreTextObj));
	}
}

HUDManager::HUDManager(mg::GameObject& owner)
	: Component(owner)
{}

HUDManager::~HUDManager()
{
	// HudManager spawns the ui elements, therefore it also removes them
	auto& context = GameContext::Instance();

	for (auto* ui : m_pPlayerInfoElements)
	{
		context.OnScoreChanged.RemoveListener(ui);
		context.OnLivesChanged.RemoveListener(ui);
	}
}
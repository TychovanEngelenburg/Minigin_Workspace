#include "HudManager.h"

#include "Game/Components/UI/PlayerUI.h"
#include "Game/Core/GameContext.h"
#include "Game/Config/FileLocations.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/Scene/Scene.h>
#include <Minigin/Components/TextComponent.h>
#include <Minigin/Rendering/SDLRenderer.h>
void HUDManager::Start()
{
	auto& context = GameContext::Instance();

	for (size_t i = 0; i < context.ActivePlayerCount(); i++)
	{
		auto hudObj = std::make_unique<mg::GameObject>("PlayerUI_P" + std::to_string(i));
		auto& ui = hudObj->AddComponent<PlayerUI>();
		ui.SetPlayerId(context.GetPlayer(i).PlayerId);

		// This could be handled in this class instead. Or with a "OnTankDeath" but this is a better demonstration.
		context.OnScoreChanged.AddListener(&ui);
		context.OnLivesChanged.AddListener(&ui);

		auto livesTextObj = std::make_unique<mg::GameObject>("LivesUI_P" + std::to_string(i));
		auto& livesText = livesTextObj->AddComponent<mg::TextComponent>(FileLocations::JoystixFont, m_fontSize);
		ui.SetSLivesComp(&livesText);
		livesTextObj->Transform().SetParent(&this->Object()->Transform());
		livesTextObj->Transform().SetLocalPosition({ mg::Renderer::Instance().WindowSize().x / context.ActivePlayerCount() * i, 0.f});

		auto scoreTextObj = std::make_unique<mg::GameObject>("ScoreUI_P" + std::to_string(i));
		auto& scoreText = scoreTextObj->AddComponent<mg::TextComponent>(FileLocations::JoystixFont, m_fontSize);
		ui.SetScoreComp(&scoreText);
		scoreTextObj->Transform().SetParent(&livesTextObj->Transform());
		scoreTextObj->Transform().Translate({ 0, 40 });


		m_pPlayerInfoElements.push_back(&ui);

		Object()->Scene()->Add(std::move(hudObj));
		Object()->Scene()->Add(std::move(livesTextObj));
		Object()->Scene()->Add(std::move(scoreTextObj));
	}
}

HUDManager::HUDManager(mg::GameObject& owner)
	: Component(owner)
{
}

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
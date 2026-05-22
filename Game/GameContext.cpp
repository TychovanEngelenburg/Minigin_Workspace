#include "GameContext.h"
#include "Minigin/Scene/SceneManager.h"
#include "SceneLoading.h"

GameContext::Mode const& GameContext::GetMode() const noexcept
{
	return m_mode;
}

void GameContext::LoadMainMenu()
{
	if (m_menuScene == m_currentLevelScene
		|| m_currentLevelScene == m_scoreboardScene
		|| !mg::SceneManager::Instance().GetScene(m_menuScene))
	{
		m_menuScene = mg::SceneManager::Instance().CreateScene();
		SceneLoading::LoadMainMenuScene(*mg::SceneManager::Instance().GetScene(m_menuScene));
	}

	mg::SceneManager::Instance().SetActiveScene(m_menuScene);
}

void GameContext::StartGame(/*Mode mode*/)
{
	//TODO: actually implement this function with gamemodes.

	if (m_currentLevelScene == m_menuScene
		|| m_currentLevelScene == m_scoreboardScene
		|| !mg::SceneManager::Instance().GetScene(m_currentLevelScene))
	{
		m_currentLevelScene = mg::SceneManager::Instance().CreateScene();
	}
	else
	{
		mg::SceneManager::Instance().CreateSceneAt(m_currentLevelScene);
	}
	SceneLoading::LoadTestScene(*mg::SceneManager::Instance().GetScene(m_currentLevelScene));

	mg::SceneManager::Instance().SetActiveScene(m_currentLevelScene);
}

void GameContext::NextLevel()
{
	// TODO: Actually implement this

	//m_currentLevel += m_currentLevel % m_levels.size();
}

void GameContext::LoadScoreboard()
{
	// TODO: Actually implement this.
}

void GameContext::SetState(std::unique_ptr<IGameState> newState)
{
	if (m_state)
	{
		m_state->OnExit();
	}

	m_state = std::move(newState);
	m_state->OnEnter();
}

void GameContext::HandleEvent(GameEvent const& event)
{
	if (!m_state) 
	{
		return;
	}

	if (auto newState = m_state->HandleEvent(event))
	{
		SetState(std::move(newState));
	}
}

GameContext::GameContext()
	: m_mode{Mode::Singleplayer}
	, m_currentLevelScene {}
	, m_menuScene{}
	, m_levelScene{}
	, m_scoreboardScene{}
	, m_state{}
{
}


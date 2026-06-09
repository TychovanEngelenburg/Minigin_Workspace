#include "GameStates.h"

#include "Game/Core/GameContext.h"
#include "Game/Core/SceneLoading.h"

#include <Minigin/Scene/SceneManager.h>
#include <cassert>

namespace
{
	mg::Scene& MakeFreshScene()
	{
		auto& scebeNabager = mg::SceneManager::Instance();
		size_t sceneId = scebeNabager.CreateScene();
		scebeNabager.SetActiveScene(sceneId);
		return *scebeNabager.GetScene(sceneId);
	}
}


#pragma region MainMenuState
void MainMenuState::OnEnter()
{
	auto& scene = MakeFreshScene();
	SceneLoading::LoadMainMenuScene(scene);
}

std::unique_ptr<GameState> MainMenuState::HandleGameEvent(GameEvent const& event)
{
	if (event == GameEvent::StartPlaying)
	{
		auto& context = GameContext::Instance();

		context.SetupGame(context.Mode());
		return std::make_unique<PlayingState>();
	}

	return nullptr;
}
#pragma endregion

#pragma region PlayingState
void PlayingState::OnEnter()
{
	auto& context = GameContext::Instance();
	auto& scene = MakeFreshScene();

	auto [players, enemies] = SceneLoading::LoadLevelScene( scene, context.Levels()[context.CurrentLevel()].File, context.Mode());

	m_playersAlive = players;
	m_enemiesAlive = enemies;
	assert(m_playersAlive > 0 && "Level loaded with no player spawns.");
	assert(m_enemiesAlive > 0 && "Level loaded with no enemy spawns.");

	context.BroadcastPlayerState();
}

std::unique_ptr<GameState> PlayingState::HandleGameEvent(GameEvent const& event)
{
	switch (event)
	{
		case GameEvent::EnemyKilled:
		{
			--m_enemiesAlive;

			if (IsWin())
			{
				GameContext::Instance().AdvanceLevel();
				return std::make_unique<PlayingState>();
			}

			break;
		}


		case GameEvent::PlayerDied:
		{
			--m_playersAlive;

			if (m_playersAlive <= 0)
			{
				return std::make_unique<PlayingState>();
			}
			if (IsGameOver())
			{
				return std::make_unique<GameOverState>();
			}

			break;
		}
	}

	return nullptr;

}

bool PlayingState::IsWin() const
{
	return m_enemiesAlive <= 0;
}

bool PlayingState::IsGameOver() const
{
	auto& context = GameContext::Instance();

	for (int i = 0; i < context.ActivePlayerCount(); i++)
	{
		if (context.GetPlayer(i).Lives > 0)
		{
			return false;
		}
	}

	return true;
}
#pragma endregion

#pragma region GameOverState
void GameOverState::OnEnter()
{
	auto& scene = MakeFreshScene();
	SceneLoading::LoadScoreboardScene(scene);
}

std::unique_ptr<GameState> GameOverState::HandleGameEvent(GameEvent const& event)
{
	if (event == GameEvent::scoreSaved)
	{
		return std::make_unique<MainMenuState>();
	}

	return nullptr;
}
#pragma endregion

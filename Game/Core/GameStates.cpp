#include "GameStates.h"

#include "Game/Core/GameContext.h"
#include "Game/Core/SceneLoading.h"

#include <Minigin/Scene/SceneManager.h>
#include <cassert>

namespace
{
	mg::Scene& MakeFreshScene()
	{
		auto& sceneManager = mg::SceneManager::Instance();

		size_t sceneId{ 0 };
		
		sceneManager.CreateSceneAt(sceneId);
		sceneManager.SetActiveScene(sceneId);
		return *sceneManager.GetScene(sceneId);
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
	if (event == GameEvent::Continue)
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

	auto [players, enemies] = SceneLoading::LoadLevelScene( scene, context.Levels()[context.CurrentLevel()]);

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

			if (IsLose())
			{
				if (IsGameOver())
				{
					if (GameContext::Instance().TotalScore() == 0)
					{
						return std::make_unique<ScoreBoardScene>();
					}

					return std::make_unique<ScoreSavingState>();
				}
				return std::make_unique<PlayingState>();
			}
	
			break;
		}

		case GameEvent::Continue:
		{
			GameContext::Instance().AdvanceLevel();
			return std::make_unique<PlayingState>();
		}
	}

	return nullptr;

}

bool PlayingState::IsWin() const
{
	return m_enemiesAlive <= 0;
}

bool PlayingState::IsLose() const
{
	return m_playersAlive <= 0;
}

bool PlayingState::IsGameOver() const
{
	for (auto const& player : GameContext::Instance().Players())
	{
		if (player.Lives > 0)
		{
			return false;
		}
	}

	return true;
}
#pragma endregion

#pragma region ScoreSaving
void ScoreSavingState::OnEnter()
{
	auto& scene = MakeFreshScene();
	SceneLoading::LoadScoreSavingScene(scene);
}

std::unique_ptr<GameState> ScoreSavingState::HandleGameEvent(GameEvent const& event)
{
	switch (event)
	{
		case GameEvent::Continue:
		{
			return std::make_unique<ScoreBoardScene>();
		}

		default:
			break;
	}

	return nullptr;
}
#pragma endregion

#pragma region ScoreBoard

#pragma endregion

void ScoreBoardScene::OnEnter()
{
	auto& scene = MakeFreshScene();
	SceneLoading::LoadScoreboardScene(scene);
}

std::unique_ptr<GameState> ScoreBoardScene::HandleGameEvent(GameEvent const& event)
{
	if (event == GameEvent::Continue)
	{
		return std::make_unique<MainMenuState>();
	}
	return nullptr;
}

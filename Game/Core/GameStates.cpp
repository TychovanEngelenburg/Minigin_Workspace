#include "GameStates.h"

#include "Game/Core/GameContext.h"

#include <memory>

#pragma region MainMenuState
void MainMenuState::OnEnter()
{
	GameContext::Instance().LoadMainMenu();
}

std::unique_ptr<IGameState> MainMenuState::HandleEvent(GameEvent const& event)
{
	switch (event)
	{
		case GameEvent::StartPlaying:
		{
			GameContext::Instance().StartGame();
			return std::make_unique<PlayingState>();
		}

		default:
		{
			return nullptr;
		}
	}
}
#pragma endregion



#pragma region PlayingState
std::unique_ptr<IGameState> PlayingState::HandleEvent(GameEvent const& event)
{
	switch (event)
	{
		//case GameEvent::PlayerDied:
		//{
		//	return std::make_unique<GameOverState>();
		//}

		//case GameEvent::AllEnemiesDead:
		//{
		//	GameContext::Instance().NextLevel();

		//	return nullptr;
		//}

		case GameEvent::QuitToMenu:
		{
			return std::make_unique < MainMenuState>();
		}

		default:
		{
			return nullptr;
		}
	}
}
#pragma endregion



#pragma region GameOverState
void GameOverState::OnEnter()
{
	GameContext::Instance().LoadScoreboard();
}

std::unique_ptr<IGameState> GameOverState::HandleEvent(GameEvent const& event)
{
	switch (event)
	{
		case GameEvent::QuitToMenu:
		{
			return std::make_unique < MainMenuState>();
		}

		case GameEvent::PlayAgain:
		{
			auto& gameContext{ GameContext::Instance() };
			gameContext.StartGame();

			return std::make_unique < PlayingState>();
		}

		default:
		{
			return nullptr;
		}
	}
}
#pragma endregion

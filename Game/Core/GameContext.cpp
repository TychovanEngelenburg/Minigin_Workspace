#include "GameContext.h"

#include "Game/Core/SceneLoading.h"

#include <Minigin/Scene/SceneManager.h>

#include <cassert>
#include <ranges>
#include <algorithm>

GameMode GameContext::Mode() const noexcept
{
	return m_mode;
}

std::vector<GameContext::LevelDefinition> const& GameContext::Levels() const
{
	return m_levels;
}

size_t GameContext::CurrentLevel() const noexcept
{
	return m_currentLevel;
}

int GameContext::CurrentScore() const
{

	int score{};
	if (m_mode != GameMode::Versus)
	{
		for (auto const& playerSession : m_players)
		{
			score += playerSession.Score;
		}
	}
	else
	{
		score = m_players[0].Score;
	}
	return score;
}

HighScoreManager& GameContext::ScoreManager() const
{
	return *m_pScoreManager;
}

/// <summary>
/// Return the amount of players whom are playing on the "player" team.
/// </summary>
/// <returns></returns>
size_t GameContext::ActivePlayerCount() const
{
	if (m_mode == GameMode::Versus)
	{
		return 1;
	}

	return m_players.size();
}

PlayerSession const& GameContext::GetPlayer(size_t index)
{
	assert(index < m_players.size());
	return m_players[index];
}


void GameContext::AdvanceLevel()
{
	assert(!m_levels.empty());
	m_currentLevel = ++m_currentLevel % m_levels.size();
}

void GameContext::SetupGame(GameMode const& mode)
{
	m_mode = mode;
	m_currentLevel = 0;

	for (size_t i = 0; i < m_players.size(); ++i)
	{
		m_players[i] = PlayerSession{};

		m_players[i].PlayerId = static_cast<int>(i);
		m_players[i].Lives = m_startingLives;

	}


	switch (mode)
	{
		case GameMode::Singleplayer:
		case GameMode::Coop:
		case GameMode::Versus:
		default:
		{
			m_levels =
			{
				{ "LevelData/01.lvl" }//,
				//{ "LevelData/02.lvl" },
				//{ "LevelData/03.lvl" }
			};
			break;
		}
	}
}

void GameContext::Init()
{
	TransitionTo(std::make_unique<MainMenuState>());
	m_pScoreManager = std::make_unique<HighScoreManager>();

	m_pScoreManager->Load();
}

void GameContext::ToggleGamemode()
{
	int modeId = static_cast<int>(m_mode);
	modeId = ++modeId % static_cast<int>(GameMode::end);

	m_mode = static_cast<GameMode>(modeId);
}

void GameContext::BroadcastPlayerState()
{
	for (auto const& player : m_players)
	{
		OnScoreChanged.Notify({
			player.PlayerId,
			player.Score
			});

		OnLivesChanged.Notify({
			player.PlayerId,
			player.Lives
			});
	}
}

#include <iostream>
void GameContext::OnNotify(TankDeathEvent const& eventData)
{
	if (eventData.KillingPlayer.has_value())
	{
		auto it = std::ranges::find_if(m_players,
			[eventData](PlayerSession const& p)
			{
				return p.PlayerId == eventData.KillingPlayer.value();
			});

		if (it != m_players.end())
		{
			it->Score += eventData.ScoreValue;
			OnScoreChanged.Notify({ it->PlayerId, it->Score });

			std::cout << "Player " << eventData.KillingPlayer.value() << " recieved: " << eventData.ScoreValue << " points. Their total is now: " << it->Score << "\n";
		}
	}



	if (eventData.PlayerVictim.has_value())
	{
		auto it = std::ranges::find_if(m_players,
			[eventData](PlayerSession const& p)
			{
				return p.PlayerId == eventData.PlayerVictim.value();
			});

		if (it != m_players.end())
		{
			--it->Lives;
			OnLivesChanged.Notify({ it->PlayerId, it->Lives });
			HandleGameEvent(GameEvent::PlayerDied);
			std::cout << "Player " << eventData.PlayerVictim.value() << " died! They now have: " << it->Lives << " left\n";
		}

	}
	else
	{
		HandleGameEvent(GameEvent::EnemyKilled);
		std::cout << "An enemy died!\n";
	}
}

void GameContext::HandleGameEvent(GameEvent const& event)
{
	m_eventQueue.push_back(event);

}

void GameContext::FlushEvents()
{
	std::vector<GameEvent> currentEvents{};
	std::swap(m_eventQueue, currentEvents);

	if (!m_pState)
	{
		return;
	}
	for (auto& event : currentEvents)
	{
		if (auto newState = m_pState->HandleGameEvent(event))
		{
			TransitionTo(std::move(newState));
		}
	}
}

GameContext::~GameContext()
{
	m_pScoreManager->Save();
}


void GameContext::TransitionTo(std::unique_ptr<GameState> state)
{
	if (m_pState)
	{
		m_pState->OnExit();
	}

	m_pState = std::move(state);

	if (m_pState)
	{
		m_pState->OnEnter();
	}
}
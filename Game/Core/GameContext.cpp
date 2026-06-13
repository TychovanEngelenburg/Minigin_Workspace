#include "GameContext.h"

#include <Minigin/Input/InputServiceLocator.h>

#include <cassert>
#include <algorithm>

PlayerDeviceMapper const& GameContext::InputMap() const
{
	return m_inputMap;
}

GameMode GameContext::Mode() const noexcept
{
	return m_mode;
}

std::vector<std::filesystem::path> const& GameContext::Levels() const
{
	return m_levels;
}

size_t GameContext::CurrentLevel() const noexcept
{
	return m_currentLevel;
}

int GameContext::TotalScore() const
{

	int score{};

	for (auto const& player : m_players)
	{
		if (!player.AsEnemy)
		{
			score += player.Score;
		}
	}

	return score;
}

ScoreWriter& GameContext::ScoreManager() const
{
	return *m_pScoreManager;
}

std::vector<PlayerSession> const& GameContext::Players()
{
	return m_players;
}


void GameContext::AdvanceLevel()
{
	assert(!m_levels.empty());
	m_currentLevel = ++m_currentLevel % m_levels.size();
}

void GameContext::SetupGame(GameMode const& mode)
{
	m_players.clear();
	m_mode = mode;
	m_currentLevel = 0;

	PlayerSession playerOne{ 0, false };
	playerOne.Lives = m_startingLives;
	m_players.push_back(std::move(playerOne));

	switch (mode)
	{
		case GameMode::Coop:
		{
			for (int i = 1; i < m_maxPlayers; ++i)
			{
				PlayerSession player{i, false };
				player.Lives = m_startingLives;

				m_players.push_back(std::move(player));
			}
			break;
		}

		case GameMode::Versus:
		{
			for (int i{ 1 }; i < m_maxPlayers; ++i)
			{
				PlayerSession player{ i, true };
				playerOne.Lives = m_startingLives;

				m_players.push_back(std::move(player));
			}
			break;
		}

		case GameMode::Singleplayer:
		default:
		{
			break;
		}
	}

	// Levels
	switch (mode)
	{
		case GameMode::Coop:
		{
			m_levels = LevelFiles::Singleplayer;

			break;
		}

		case GameMode::Versus:
		{
			m_levels = LevelFiles::Versus;

			break;
		}

		case GameMode::Singleplayer:
		default:
		{
			m_levels = LevelFiles::Singleplayer;

			break;
		}
	}

	m_inputMap.Resolve(mg::InputServiceLocator::Fetch(), m_players.size());
}

void GameContext::Init()
{
	TransitionTo(std::make_unique<MainMenuState>());
	m_pScoreManager = std::make_unique<ScoreWriter>();

	m_pScoreManager->Load();
}

void GameContext::ToggleGamemode()
{
	int modeId = static_cast<int>(m_mode);
	modeId = ++modeId % static_cast<int>(GameMode::end);

	m_mode = static_cast<GameMode>(modeId);

	OnGameModeChanged.Notify({ m_mode });
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

void GameContext::OnNotify(TankDeathEvent const& eventData)
{
	if (eventData.KillingPlayer.has_value())
	{
		auto& player = m_players[eventData.KillingPlayer.value()];
		player.Score += eventData.ScoreValue;
		OnScoreChanged.Notify({ player.PlayerId, player.Score });
	}

	if (eventData.PlayerVictim.has_value())
	{
		auto& player = m_players[eventData.PlayerVictim.value()];
		--player.Lives;
		OnLivesChanged.Notify({ player.PlayerId, player.Lives });

		PushEvent(GameEvent::PlayerDied);
	}
	else
	{
		PushEvent(GameEvent::EnemyKilled);
	}
}

void GameContext::PushEvent(GameEvent const& event)
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
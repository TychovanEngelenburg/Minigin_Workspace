#include "GameContext.h"

#include "Game/Core/SceneLoading.h"

#include <Minigin/Scene/SceneManager.h>

#include <cassert>


GameContext::GameMode const& GameContext::Mode() const noexcept
{
	return m_mode;
}

std::vector<GameContext::LevelDefinition> const& GameContext::Levels() const
{
	return m_levels;
}

size_t const& GameContext::CurrentLevel() const noexcept
{
	return m_currentLevel;
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

PlayerSession& GameContext::GetPlayer(size_t index)
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
		m_players[i] = PlayerSession{ static_cast<int>(i), 4 };
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
}

void GameContext::ToggleGamemode()
{
	int modeId = static_cast<int>(m_mode);
	modeId = ++modeId % static_cast<int>(GameMode::end);

	m_mode = static_cast<GameMode>(modeId);
}

void GameContext::HandleGameEvent(GameEvent const& event)
{
	m_eventQueue.push_back(event);

}

void GameContext::FlushEvents()
{
	std::vector<GameEvent> currentEvents{};
	std::swap(m_eventQueue, currentEvents);

	if (!m_state)
	{
		return;
	}
	for (auto& event : currentEvents)
	{
		if (auto newState = m_state->HandleGameEvent(event))
		{
			TransitionTo(std::move(newState));
			//m_pendingState = std::move(newState);
		}
	}
}


void GameContext::TransitionTo(std::unique_ptr<GameState> state)
{
	if (m_state)
	{
		m_state->OnExit();
	}

	m_state = std::move(state);

	if (m_state)
	{
		m_state->OnEnter();
	}
}
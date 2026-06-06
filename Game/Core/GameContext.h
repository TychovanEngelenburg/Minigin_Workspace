#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "Game/Core/GameStates.h"
#include "Game/Core/PlayerSession.h"
#include "Game/Events/GameEvents.h"

#include <Minigin/Core/Singleton.h>

#include <memory>
#include <array>
#include <vector>
#include <filesystem>

class GameContext final : public mg::Singleton<GameContext>
{
public:
	enum class GameMode
	{
		Singleplayer,
		Coop,
		Versus,
		end
	};

	struct LevelDefinition
	{
		std::filesystem::path File;
	};

	GameMode const& Mode() const noexcept;
	std::vector<LevelDefinition> const& Levels() const;
	size_t const& CurrentLevel() const noexcept;

	size_t ActivePlayerCount() const;
	PlayerSession& GetPlayer(size_t index);

	void AdvanceLevel();

	void SetupGame(GameMode const& mode);
	void Init();
	void ToggleGamemode();

	void HandleGameEvent(GameEvent const& event);
	void FlushEvents();

private:
	friend class Singleton<GameContext>;

	void TransitionTo(std::unique_ptr<GameState> state);

	std::unique_ptr<GameState> m_state{};
	std::vector <GameEvent> m_eventQueue{};


	std::vector<LevelDefinition> m_levels;
	std::array<PlayerSession, 2> m_players{};
	size_t m_currentLevel{};
	GameMode m_mode{ GameMode::Singleplayer };
};
#endif //GAME_CONTEXT_H
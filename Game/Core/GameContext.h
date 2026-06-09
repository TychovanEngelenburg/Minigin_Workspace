#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "Game/Core/GameStates.h"
#include "Game/Core/PlayerSession.h"
#include "Game/Events/GameEvents.h"
#include "Game/Events/UIEvents.h"

#include <Minigin/Core/Singleton.h>
#include <Minigin/Events/IObserver.h>

#include <Minigin/Events/Subject.h>

#include <memory>
#include <array>
#include <vector>
#include <filesystem>

class GameContext final : public mg::Singleton<GameContext>, public mg::IObserver<TankDeathEvent>
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

	GameMode Mode() const noexcept;
	std::vector<LevelDefinition> const& Levels() const;
	size_t CurrentLevel() const noexcept;

	size_t ActivePlayerCount() const;
	PlayerSession const& GetPlayer(size_t index);

	void AdvanceLevel();

	void SetupGame(GameMode const& mode);
	void Init();
	void ToggleGamemode();

	void BroadcastPlayerState();

	void OnNotify(TankDeathEvent const& eventData) override;
	void HandleGameEvent(GameEvent const& event);
	void FlushEvents();

	mg::Subject<ScoreChangedEvent> OnScoreChanged;
	mg::Subject<LivesChangedEvent> OnLivesChanged;

private:
	friend class Singleton<GameContext>;

	void TransitionTo(std::unique_ptr<GameState> state);

	static int constexpr m_startingLives{ 4 };

	std::unique_ptr<GameState> m_state{};
	std::vector <GameEvent> m_eventQueue{};

	std::vector<LevelDefinition> m_levels;
	std::array<PlayerSession, 2> m_players{};
	GameMode m_mode{ GameMode::Singleplayer };
	size_t m_currentLevel{0};
};
#endif //GAME_CONTEXT_H
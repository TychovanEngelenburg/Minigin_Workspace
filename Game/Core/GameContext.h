#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "Game/Core/ScoreWriter.h"
#include "Game/Core/GameStates.h"
#include "Game/Core/PlayerSession.h"
#include "Game/Core/GameModes.h"
#include "Game/Events/GameEvents.h"
#include "Game/Events/UIEvents.h"
#include "Game/Core/PlayerDeviceMapper.h"

#include <Minigin/Core/Singleton.h>
#include <Minigin/Events/IObserver.h>

#include <Minigin/Events/Subject.h>

#include <memory>
#include <vector>
#include <filesystem>


class GameContext final : public mg::Singleton<GameContext>, public mg::IObserver<TankDeathEvent>
{
public:
	PlayerDeviceMapper const& InputMap() const;

	GameMode Mode() const noexcept;
	std::vector<std::filesystem::path> const& Levels() const;
	size_t CurrentLevel() const noexcept;
	int TotalScore() const;
	ScoreWriter& ScoreManager() const;

	std::vector<PlayerSession> const& Players();


	void AdvanceLevel();

	void SetupGame(GameMode const& mode);
	void Init();
	void ToggleGamemode();

	void BroadcastPlayerState();

	void OnNotify(TankDeathEvent const& eventData) override;
	void PushEvent(GameEvent const& event);
	void FlushEvents();

	GameContext() = default;

	~GameContext() override;
	GameContext(GameContext const& other) = delete;
	GameContext(GameContext&& other) = delete;
	GameContext& operator=(GameContext const& other) = delete;
	GameContext& operator=(GameContext&& other) = delete;

	mg::Subject<ScoreChangedEvent> OnScoreChanged;
	mg::Subject<LivesChangedEvent> OnLivesChanged;
	mg::Subject<GameModeChangedEvent> OnGameModeChanged;

private:
	friend class Singleton<GameContext>;

	void TransitionTo(std::unique_ptr<GameState> state);

	static int constexpr m_startingLives{ 1 };
	static size_t constexpr m_maxPlayers{ 2 };

	PlayerDeviceMapper m_inputMap{};

	std::unique_ptr<GameState> m_pState{};
	std::vector <GameEvent> m_eventQueue{};

	std::vector<std::filesystem::path> m_levels;
	size_t m_currentLevel{0};
	
	std::vector<PlayerSession> m_players{};
	GameMode m_mode{ GameMode::Singleplayer };

	std::unique_ptr<ScoreWriter> m_pScoreManager{};
};
#endif //GAME_CONTEXT_H
#ifndef GAME_STATES_H
#define GAME_STATES_H

#include "Game/Events/GameEvents.h"

#include <memory>

class GameState
{
public:
	virtual void OnEnter() {}
	virtual void OnExit() {}

	[[nodiscard]] virtual std::unique_ptr<GameState> HandleGameEvent(GameEvent const& event) = 0;
	
	GameState() = default;

	virtual ~GameState() = default;
	GameState(GameState const& other) = delete;
	GameState(GameState&& other) = delete;
	GameState& operator=(GameState const& other) = delete;
	GameState& operator=(GameState&& other) = delete;
};

class MainMenuState final : public GameState
{
public:
	void OnEnter() override;
	std::unique_ptr<GameState> HandleGameEvent(GameEvent const& event) override;
};

class PlayingState final : public GameState
{
public:
	void OnEnter() override;
	[[nodiscard]] std::unique_ptr<GameState> HandleGameEvent(GameEvent const& event) override;

	bool IsWin() const;
	bool IsLose() const;
	bool IsGameOver() const;

private:
	int m_playersAlive{ 0 };
	int m_enemiesAlive{ 0 };
};


class ScoreSavingState final : public GameState
{
public:
	void OnEnter() override;

	[[nodiscard]] std::unique_ptr<GameState> HandleGameEvent(GameEvent const& event) override;
};

class ScoreBoardScene final : public GameState
{
public:
	void OnEnter() override;

	[[nodiscard]] std::unique_ptr<GameState> HandleGameEvent(GameEvent const& event) override;
};
#endif // GAME_STATES_H
#ifndef GAME_STATES_H
#define GAME_STATES_H

#include <memory>

enum class GameEvent
{
	PlayerDied,
	AllEnemiesDead,

	QuitToMenu,
	StartPlaying,
	PlayAgain
};

class IGameState
{
public:
	virtual void OnEnter() {}
	virtual void OnExit() {}

	virtual std::unique_ptr<IGameState> HandleEvent(GameEvent const& event) = 0;

	virtual ~IGameState() = default;
};

class MainMenuState final : public IGameState
{
public:
	void OnEnter() override;
	std::unique_ptr<IGameState> HandleEvent(GameEvent const& event) override;
};

class PlayingState final : public IGameState
{
public:
	std::unique_ptr<IGameState> HandleEvent(GameEvent const& event) override;
};


class GameOverState final : public IGameState
{
public:
	void OnEnter() override;

	std::unique_ptr<IGameState> HandleEvent(GameEvent const& event) override;
};
#endif // GAME_STATES_H
#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H
#include <vector>
#include <string>
#include "Minigin/Singleton.h"
#include "GameStates.h"
#include <memory>

class GameContext : public mg::Singleton< GameContext>
{
public:
	enum class Mode
	{
		Singleplayer,
		Coop,
		Versus
	};

	Mode const& GetMode() const noexcept;

	void LoadMainMenu();
	void StartGame(/*Mode mode*/);
	void NextLevel();
	void LoadScoreboard();

	void SetState(std::unique_ptr<IGameState> newState);
	void HandleEvent(GameEvent const& event);

	GameContext();

	virtual ~GameContext() = default;
	GameContext(GameContext const& other) = delete;
	GameContext(GameContext&& other) = delete;

	GameContext& operator=(GameContext const& other) = delete;
	GameContext& operator=(GameContext&& other) = delete;

private:
	friend class Singleton<GameContext>;


	Mode m_mode;
	size_t m_currentLevelScene;

	size_t m_menuScene;
	size_t m_levelScene;
	size_t m_scoreboardScene;

	std::unique_ptr<IGameState> m_state;

	// TODo: Level pre-caching
	// size_t m_cachedScene;

};
#endif //GAME_CONTEXT_H
#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "GameStates.h"

#include <Minigin/Core/Singleton.h>

#include <memory>

class GameContext final : public mg::Singleton<GameContext>
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

private:
	friend class Singleton<GameContext>;

	Mode m_mode{ Mode::Singleplayer };
	size_t m_currentLevelScene{};

	size_t m_menuScene{};
	size_t m_levelScene{};
	size_t m_scoreboardScene{};

	std::unique_ptr<IGameState> m_state{};

	// TODo: Level pre-caching
	// size_t m_cachedScene;

};
#endif //GAME_CONTEXT_H
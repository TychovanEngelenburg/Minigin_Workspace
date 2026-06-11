#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include "Game/Core/GameModes.h"

#include <filesystem>
#include <array>

struct PlayerScore
{
	static constexpr size_t NameLength{ 3 };
	int score{ 0 };
	std::string name{ "AAA"};
};


class HighScoreManager
{
public:
	using ScoreBoard = std::vector<PlayerScore>;

	ScoreBoard const& GetScoreboard(GameMode mode) const;

	void AddScore(GameMode mode, PlayerScore&& entry);

	void Load(std::filesystem::path const& file = "highscores.dat");
	void Save(std::filesystem::path const& file = "highscores.dat") const;

private:

	static constexpr size_t MaxEntries{ 10 }; // TODO: Add to config file eventually.
	std::array<ScoreBoard, static_cast<int>(GameMode::end)> m_scoreboards{};
};

#endif // HIGHSCOREMANAGER_H
#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include "Game/Core/GameModes.h"
#include "Game/Config/FileConfig.h"

#include <filesystem>
#include <array>

struct PlayerScore
{
	static constexpr size_t NameLength{ 3 };
	int score{ 0 };
	std::string name{ "AAA"};
};


class ScoreWriter
{
public:
	using ScoreBoard = std::vector<PlayerScore>;

	ScoreBoard const& GetScoreboard(GameMode mode) const;

	void AddScore(GameMode mode, PlayerScore&& entry);

	void Load(std::filesystem::path const& file = Files::ScoreFile);
	void Save(std::filesystem::path const& file = Files::ScoreFile) const;

private:

	static constexpr size_t MaxEntries{ 10 }; // TODO: Add to config file eventually.
	std::array<ScoreBoard, static_cast<int>(GameMode::end)> m_scoreboards{};
};

#endif // HIGHSCOREMANAGER_H
#include "HighScoreManager.h"

#include <Minigin/Rendering/ResourceManager.h>

#include <fstream>
#include <algorithm>

const HighScoreManager::ScoreBoard& HighScoreManager::GetScoreboard(GameMode mode) const
{
	return m_scoreboards[static_cast<int>(mode)];
}

void HighScoreManager::AddScore(GameMode mode,  PlayerScore&& entry)
{
	ScoreBoard& board = m_scoreboards[static_cast<int>(mode)];

	auto it = std::lower_bound(board.begin(), board.end(), entry, 
		[](PlayerScore const& a, PlayerScore const& b)
		{
			return a.score > b.score;
		});

	board.insert(it, entry);

	if (board.size() > MaxEntries)
	{
		board.resize(MaxEntries);
	}
}

#include <iostream> // TODO: remove this.
void HighScoreManager::Load(std::filesystem::path const& file)
{
	std::ifstream in(mg::ResourceManager::Instance().DataPath() / file, std::ios::binary);
	if (!in)
	{
		return;
	}

	m_scoreboards = {};
	for (ScoreBoard& board : m_scoreboards)
	{
		uint32_t count{};
		if (!in.read(std::bit_cast<char*>(&count), sizeof(count)))
		{
			return;
		}

		count = std::min(count, static_cast<uint32_t>(MaxEntries));
		for (uint32_t i = 0; i < count; ++i)
		{
			PlayerScore entry{};
			if (!in.read(std::bit_cast<char*>(&entry.score), sizeof(entry.score)))
			{
				return;
			}

			entry.name.resize(entry.NameLength);
			if (!in.read(entry.name.data(), entry.NameLength))
			{
				return;
			}

			std::cout << entry.name << " loaded with score " << entry.score << "\n";
			board.push_back(entry);
		}
	}
}

void HighScoreManager::Save(std::filesystem::path const& file) const
{
	std::ofstream out(mg::ResourceManager::Instance().DataPath() / file, std::ios::binary);
	if (!out)
	{
		return;
	}

	for (ScoreBoard const& board : m_scoreboards)
	{
		uint32_t count = static_cast<uint32_t>(board.size());

		out.write(std::bit_cast<char const*>(&count), sizeof(count));
		for (PlayerScore const& entry : board)
		{
			out.write(std::bit_cast<char const*>(&entry.score), sizeof(entry.score));
			out.write(entry.name.data(), entry.NameLength);
		}
	}
}
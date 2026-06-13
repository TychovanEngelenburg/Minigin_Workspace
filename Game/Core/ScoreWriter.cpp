#include "ScoreWriter.h"

#include <Minigin/Rendering/ResourceManager.h>

#include <fstream>
#include <algorithm>

const ScoreWriter::ScoreBoard& ScoreWriter::GetScoreboard(GameMode mode) const
{
	return m_scoreboards[static_cast<int>(mode)];
}

void ScoreWriter::AddScore(GameMode mode, PlayerScore&& entry)
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

void ScoreWriter::Load(std::filesystem::path const& file)
{
	std::ifstream in(mg::ResourceManager::Instance().DataPath() / file);
	if (!in)
	{
		return;
	}

	m_scoreboards = {};
	for (ScoreBoard& board : m_scoreboards)
	{
		std::string line;
		if (!std::getline(in, line))
		{
			return;
		}

		size_t count{};
		std::from_chars(line.data(), line.data() + line.size(), count);
		count = std::min(count, MaxEntries);

		for (size_t i = 0; i < count; ++i)
		{
			if (!std::getline(in, line))
			{
				return;
			}

			PlayerScore entry{};
			std::istringstream ss(line);
			ss >> entry.name >> entry.score;
			board.push_back(std::move(entry));
		}
	}

}

void ScoreWriter::Save(std::filesystem::path const& file) const
{
	auto path = mg::ResourceManager::Instance().DataPath() / file;
	std::filesystem::create_directories(path.parent_path());

	std::ofstream out(path);
	if (!out)
	{
		return;
	}

	for (ScoreBoard const& board : m_scoreboards)
	{
		out << board.size() << '\n';
		for (PlayerScore const& entry : board)
		{
			out << entry.name << ' ' << entry.score << '\n';
		}
	}
}
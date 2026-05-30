#include "GameGrid.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/ResourceManager.h>
#include <Minigin/Renderer.h>

#include <fstream>
#include <iostream>

GameGrid::GameGrid(mg::GameObject& owner, std::filesystem::path const& filePath, float tileSize)
	: Component(owner)
	, m_rows{}
	, m_cols{}
	, m_tiles{}
	, m_tileSize{ tileSize }
	, m_pTileSheet{}
	, m_pBackgroundTexture{}
{
	LoadFromFile(mg::ResourceManager::Instance().DataPath() / filePath);
	m_pTileSheet = mg::ResourceManager::Instance().LoadTexture("T_TileSheet_Tron_BattleTanks.png");
	m_pBackgroundTexture = mg::ResourceManager::Instance().LoadTexture("T_Motherboard_Background.png");
}

Tile* GameGrid::GetTile(glm::ivec2 const& gridPos)
{
	if (gridPos.x >= m_cols || gridPos.x < 0 ||
		gridPos.y >= m_rows || gridPos.y < 0)
	{
		return nullptr;
	}


	return &m_tiles[m_cols * gridPos.y + gridPos.x];
}

Tile const* GameGrid::GetTile(glm::ivec2 const& gridPos) const
{
	if (gridPos.x >= m_cols || gridPos.x < 0 ||
		gridPos.y >= m_rows || gridPos.y < 0)
	{
		return nullptr;
	}


	return &m_tiles[m_cols * gridPos.y + gridPos.x];
}


std::vector<glm::ivec2> const& GameGrid::PlayerSpawnpoints() const noexcept
{
	return m_playerSpawns;
}

std::vector<glm::ivec2> const& GameGrid::EnemySpawnpoints() const noexcept
{
	return m_enemySpawns;
}

float GameGrid::TileSize() const noexcept
{
	return m_tileSize;
}

bool GameGrid::WallAt(glm::ivec2 const& gridPos) const
{
	auto tile = GetTile(gridPos);
	if (!tile)
	{
		return true;
	}

	return tile->IsWall;
}

bool GameGrid::IsPath(glm::ivec2 const& gridPos) const
{
	return(!WallAt(gridPos)
		&& !WallAt({ gridPos.x + 1, gridPos.y })
		&& !WallAt({ gridPos.x + 1, gridPos.y + 1 })
		&& !WallAt({ gridPos.x, gridPos.y + 1 })
		);
}

glm::ivec2 GameGrid::WorldToGrid(glm::vec2 const& worldPos) const
{
	return glm::ivec2(
		floor((worldPos.x - Owner()->Transform().WorldPosition().x) / m_tileSize),
		floor((worldPos.y - Owner()->Transform().WorldPosition().y) / m_tileSize)
	);
}

glm::vec2 GameGrid::GridToWorld(glm::ivec2 const& gridPos) const
{
	return glm::vec2(Owner()->Transform().WorldPosition() + static_cast<glm::vec2>(gridPos) * m_tileSize);
}


glm::ivec2 GameGrid::IdToGrid(int idx) const
{
	return glm::ivec2(
		idx % m_cols,
		idx / m_cols
	);
}


void GameGrid::Render() const
{
	glm::vec2 gridPos{ Owner()->Transform().LocalPosition() };
	SDL_FRect bgDst{ gridPos.x, gridPos.y, m_tileSize * m_cols, m_tileSize * m_rows };
	mg::Renderer::Instance().RenderTexture(*m_pBackgroundTexture, bgDst);


	SDL_FRect src{ 0, 0 , m_tileTexSize, m_tileTexSize };
	SDL_FRect dst{ 0, 0, m_tileSize, m_tileSize };

	for (size_t tileId = 0; tileId < m_tiles.size(); tileId++)
	{

		if (m_tiles[tileId].IsWall)
		{
			continue;
		}

		if (m_tiles[tileId].Walkable)
		{
			src.x = m_tileTexSize * static_cast<float>(m_tiles[tileId].PathConnections);
			src.y = m_tileTexSize * m_walkableTileY;
		}
		else
		{
			src.x = 0;
			src.y = 0;
		}

		dst.x = gridPos.x + IdToGrid(static_cast<int>(tileId)).x * m_tileSize;
		dst.y = gridPos.y + IdToGrid(static_cast<int>(tileId)).y * m_tileSize;
		mg::Renderer::Instance().RenderTexture(*m_pTileSheet, dst, src);
	}


}

void GameGrid::ComputeWalkables()
{
	for (int x = 0; x < m_cols; x++)
	{
		for (int y = 0; y < m_rows; y++)
		{
			m_tiles[m_cols * y + x].Walkable = (
				!WallAt({ x, y }) &&
				!WallAt({ x + 1, y }) &&
				!WallAt({ x + 1, y + 1 }) &&
				!WallAt({ x, y + 1 })
				);
		}
	}
}

void GameGrid::ComputeConnections()
{
	for (int x = 0; x < m_cols; x++)
	{
		for (int y = 0; y < m_rows; y++)
		{

			Tile const* left = GetTile({ x - 1, y });
			Tile const* up = GetTile({ x, y - 1 });

			bool hor = left && left->Walkable;
			bool vert = up && up->Walkable;

			if (hor && vert)
			{
				GetTile({ x, y })->PathConnections = Tile::ConnectionMask::both;
			}
			else if (hor)
			{
				GetTile({ x, y })->PathConnections = Tile::ConnectionMask::horizontal;
			}
			else if (vert)
			{
				GetTile({ x, y })->PathConnections = Tile::ConnectionMask::vertical;
			}
			else
			{
				GetTile({ x, y })->PathConnections = Tile::ConnectionMask::none;
			}
		}
	}
}

void GameGrid::ProcessLine(std::string const& line)
{

	if (line.empty())
	{
		return;
	}

	if (m_cols == 0)
	{
		m_cols = static_cast<int>(line.length());
	}

	for (size_t col = 0; col < line.length(); ++col)
	{
		const char tile = line[col];

		glm::ivec2 gridPos
		{
			static_cast<int>(col),
			m_rows
		};

		switch (tile)
		{
			case '.':
			{
				m_tiles.push_back(Tile{ false, false });
				break;
			}

			case 'P':
			{
				m_tiles.push_back(Tile{ false, false });
				m_playerSpawns.push_back(gridPos);
				break;
			}

			case 'E':
			{
				m_tiles.push_back(Tile{ false, false });
				m_enemySpawns.push_back(gridPos);
				break;
			}

			default:
			{
				m_tiles.push_back(Tile{ true, false });
				break;
			}
		}
	}

	++m_rows;

}

void GameGrid::LoadFromFile(std::filesystem::path const& filePath)
{
	m_rows = 0;
	m_cols = 0;

	m_tiles.clear();
	m_playerSpawns.clear();
	m_enemySpawns.clear();

	std::ifstream file(filePath, std::ios::binary);

	if (!file)
	{
		throw std::runtime_error("Failed to open grid file");
	}

	std::string line;
	char ch{};

	while (file.get(ch))
	{
		if (ch == '\n' || ch == '\r')
		{
			ProcessLine(line);
			line.clear();
		}
		else
		{
			line += ch;
		}
	}
	ProcessLine(line);

	ComputeWalkables();
	ComputeConnections();

	for (int y = 0; y < m_rows; y++)
	{
		for (int x = 0; x < m_cols; x++)
		{
			std::cout << (GetTile({ x, y })->Walkable ? "." : (GetTile({ x, y })->IsWall ? "#" : " "));
		}
		std::cout << '\n';
	}
}

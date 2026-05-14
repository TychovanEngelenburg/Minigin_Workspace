#include "GameGrid.h"
#include <fstream>
#include "Minigin/GameObject.h"
#include "Minigin/ResourceManager.h"
#include "Minigin/Renderer.h"
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

Tile* GameGrid::GetTile(int gridX, int gridY)
{
	if (gridX >= m_cols || gridX < 0 ||
		gridY >= m_rows || gridY < 0)
	{
		return nullptr;
	}


	return &m_tiles[m_cols * gridY + gridX];
}

Tile const* GameGrid::GetTile(int gridX, int gridY) const
{
	if (gridX >= m_cols || gridX < 0 ||
		gridY >= m_rows || gridY < 0)
	{
		return nullptr;
	}


	return &m_tiles[m_cols * gridY + gridX];
}


float GameGrid::TileSize() const noexcept
{
	return m_tileSize;
}

bool GameGrid::WallAt(int gridX, int gridY) const
{
	auto tile = GetTile(gridX, gridY);
	if (!tile)
	{
		return true;
	}

	return tile->IsWall;
}

bool GameGrid::IsPath(int gridX, int gridY) const
{
	return(!WallAt(gridX, gridY)
		&& !WallAt(gridX + 1, gridY)
		&& !WallAt(gridX + 1, gridY + 1)
		&& !WallAt(gridX, gridY + 1)
		);
}

glm::ivec2 GameGrid::WorldToGrid(float worldX, float worldY) const
{
	return glm::ivec2(
		floor((worldX - Owner()->Transform().WorldPosition().x) / m_tileSize),
		floor((worldY - Owner()->Transform().WorldPosition().y) / m_tileSize)
	);
}

glm::vec2 GameGrid::GridToWorld(int gridX, int gridY) const
{
	return glm::vec2(
		Owner()->Transform().WorldPosition().x + m_tileSize * gridX,
		Owner()->Transform().WorldPosition().y + m_tileSize * gridY
	);
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
	glm::vec3 gridPos{ Owner()->Transform().LocalPosition() };
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

void GameGrid::LoadFromFile(std::filesystem::path const& filePath)
{
	m_rows = 0;
	m_cols = 0;
	m_tiles.clear();

	std::ifstream file(filePath, std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open grid file");
	}

	std::string allText(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>()
	);

	std::string currentLine{};

	for (char character : allText)
	{
		if (character == '\n' || character == '\r')
		{
			if (!currentLine.empty())
			{
				if (m_cols == 0)
				{
					m_cols = static_cast<int>(currentLine.length());
				}

				++m_rows;

				for (char tile : currentLine)
				{
					if (tile == '.')
					{
						m_tiles.push_back(Tile{ false, false });
					}
					else
					{
						m_tiles.push_back(Tile{ true, false });
					}
				}

				currentLine.clear();
			}
		}
		else
		{
			currentLine += character;
		}
	}

	if (!currentLine.empty())
	{
		if (m_cols == 0)
		{
			m_cols = static_cast<int>(currentLine.length());
		}

		++m_rows;

		for (char tile : currentLine)
		{
			if (tile == '.')
			{
				m_tiles.push_back(Tile{ false, false });
			}
			else
			{
				m_tiles.push_back(Tile{ true, false });
			}
		}
	}

	ComputeWalkables();
	ComputeConnections();

	for (int y = 0; y < m_rows; y++)
	{
		for (int x = 0; x < m_cols; x++)
		{
			std::cout << (GetTile(x, y)->Walkable ? "." : (GetTile(x, y)->IsWall ? "#" : " "));
		}
		std::cout << '\n';
	}
}
void GameGrid::ComputeWalkables()
{
	for (int x = 0; x < m_cols; x++)
	{
		for (int y = 0; y < m_rows; y++)
		{
			m_tiles[m_cols * y + x].Walkable = (
				!WallAt(x, y) &&
				!WallAt(x + 1, y) &&
				!WallAt(x + 1, y + 1) &&
				!WallAt(x, y + 1)
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

			Tile const* left = GetTile(x - 1, y);
			Tile const* up = GetTile(x, y - 1);

			bool hor = left && left->Walkable;
			bool vert = up && up->Walkable;

			if (hor && vert)
			{
				GetTile(x, y)->PathConnections = Tile::ConnectionMask::both;
			}
			else if (hor)
			{
				GetTile(x, y)->PathConnections = Tile::ConnectionMask::horizontal;
			}
			else if (vert)
			{
				GetTile(x, y)->PathConnections = Tile::ConnectionMask::vertical;
			}
			else
			{
				GetTile(x, y)->PathConnections = Tile::ConnectionMask::none;
			}
		}
	}
}
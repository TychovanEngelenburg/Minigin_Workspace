#include "GameGrid.h"

#include "Game/Config/FileConfig.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/Rendering/ResourceManager.h>
#include <Minigin/Rendering/SDLRenderer.h>
#include <Minigin/Rendering/SourceRect.h>
#include <Minigin/Rendering/SDLTexture2D.h>

#include <fstream>
#include <iostream>

GameGrid::GameGrid(mg::GameObject& owner, std::filesystem::path const& filePath, float tileSize)
	: Component(owner)
	, m_tileSize(tileSize)
{
	LoadFromFile(mg::ResourceManager::Instance().DataPath() / filePath);
	m_pTileSheet = mg::ResourceManager::Instance().LoadTexture(Files::GridTileSheet.filePath);
	m_pBackgroundTexture = mg::ResourceManager::Instance().LoadTexture(Files::GridBackground);

	m_tileTexSize = m_pTileSheet->Size().x / Files::GridTileSheet.cols;
}

Tile* GameGrid::GetTile(glm::ivec2 const& gridPos) 
{
	if (gridPos.x >= m_gridSize.x || gridPos.x < 0 ||
		gridPos.y >= m_gridSize.y || gridPos.y < 0)
	{
		return nullptr;
	}


	return &m_tiles[m_gridSize.x * gridPos.y + gridPos.x];
}

Tile const* GameGrid::GetTile(glm::ivec2 const& gridPos) const
{
	if (gridPos.x >= m_gridSize.x || gridPos.x < 0 ||
		gridPos.y >= m_gridSize.y || gridPos.y < 0)
	{
		return nullptr;
	}


	return &m_tiles[m_gridSize.x * gridPos.y + gridPos.x];
}

std::vector<GameGrid::TankSpawnPoint> const& GameGrid::TankSpawnpoints() const noexcept
{
	return m_tankSpawns;
}

glm::ivec2 const& GameGrid::TeleporterPos() const noexcept
{
	return m_teleporterGridPos;
}

float GameGrid::TileSize() const noexcept
{
	return m_tileSize;
}

glm::ivec2 const& GameGrid::Size() const noexcept
{
	return m_gridSize;
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
		floor((worldPos.x - Object()->Transform().WorldPosition().x) / m_tileSize),
		floor((worldPos.y - Object()->Transform().WorldPosition().y) / m_tileSize)
	);
}

glm::vec2 GameGrid::GridToWorld(glm::ivec2 const& gridPos) const
{
	return glm::vec2(Object()->Transform().WorldPosition() + static_cast<glm::vec2>(gridPos) * m_tileSize);
}


glm::ivec2 GameGrid::IdToGrid(int idx) const
{
	return glm::ivec2(
		idx % m_gridSize.x,
		idx / m_gridSize.x
	);
}


void GameGrid::Render() const
{
	// Draw background
	glm::vec2 gridPos{ Object()->Transform().LocalPosition() };
	glm::vec2 gridSize{ m_tileSize * static_cast<glm::vec2>(m_gridSize) };
	SourceRect bgDst{ gridPos.x, gridPos.y, gridSize.x, gridSize.y };
	mg::Renderer::Instance().RenderTexture(*m_pBackgroundTexture, bgDst);

	// Draw tiles
	SourceRect src{ 0, 0 , m_tileTexSize, m_tileTexSize };
	SourceRect dst{ 0, 0, m_tileSize, m_tileSize };

	for (size_t tileId = 0; tileId < m_tiles.size(); tileId++)
	{

		if (m_tiles[tileId].IsWall)
		{
			continue;
		}

		if (m_tiles[tileId].Walkable)
		{
			src.x = m_tileTexSize * static_cast<float>(m_tiles[tileId].PathConnections);
			src.y = 0;// m_tileTexSize * m_walkableTileY;
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
	for (int x = 0; x < m_gridSize.x; x++)
	{
		for (int y = 0; y < m_gridSize.y; y++)
		{
			m_tiles[m_gridSize.x * y + x].Walkable = (
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
	for (int x = 0; x < m_gridSize.x; x++)
	{
		for (int y = 0; y < m_gridSize.y; y++)
		{

			auto const* left = GetTile({ x - 1, y });
			auto const* up = GetTile({ x, y - 1 });

			bool hor{ left && left->Walkable };
			bool vert{ up && up->Walkable
			};

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

	if (m_gridSize.x == 0)
	{
		m_gridSize.x = static_cast<int>(line.length());
	}

	for (size_t col = 0; col < line.length(); ++col)
	{
		char const tile = line[col];

		glm::ivec2 gridPos
		{
			static_cast<int>(col),
			m_gridSize.y
		};

		switch (tile)
		{
			case '#':
			{
				m_tiles.push_back(Tile(true, false));
				break;
			}

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			{
				m_tiles.push_back(Tile( false, false ));

				TankSpawnPoint tankSpawn{};
				tankSpawn.GridPos = gridPos;
				tankSpawn.Type = TankSpawnType::Player;
				tankSpawn.PlayerId = static_cast<int>(tile - '0');
				m_tankSpawns.push_back(std::move(tankSpawn));
				
				break;
			}

			case 'E':
			{
				m_tiles.push_back(Tile( false, false ));

				TankSpawnPoint tankSpawn{};
				tankSpawn.GridPos = gridPos;
				tankSpawn.Type = TankSpawnType::BasicEnemy;
				m_tankSpawns.push_back(std::move(tankSpawn));
				
				break;
			}

			case 'R':
			{
				m_tiles.push_back(Tile(false, false));

				TankSpawnPoint tankSpawn{};
				tankSpawn.GridPos = gridPos;
				tankSpawn.Type = TankSpawnType::Recogniser;
				m_tankSpawns.push_back(std::move(tankSpawn));

				break;
			}

			case 'T':
			{
				m_tiles.push_back(Tile(false, false));
				m_teleporterGridPos = gridPos;

				break;
			}

			default:
			{
				m_tiles.push_back(Tile(false, false));
				break;
			}
		}
	}

	++m_gridSize.y;
}

void GameGrid::LoadFromFile(std::filesystem::path const& filePath)
{
	m_gridSize = {};

	m_tiles.clear();
	m_tankSpawns.clear();

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
}
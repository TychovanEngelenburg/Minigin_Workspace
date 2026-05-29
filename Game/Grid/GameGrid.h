#ifndef GAMEGRID_H
#define GAMEGRID_H

#include <Minigin/EngineComponents/Component.h>
#include <Minigin/SDL_Implementation/SDLTexture2D.h>

#include <vector>
#include <filesystem>
#include <memory>

struct Tile
{

	bool IsWall;
	bool Walkable;
	enum class ConnectionMask
	{
		none,
		horizontal,
		vertical,
		both
	};
	ConnectionMask PathConnections;
};

class GameGrid final : public mg::Component
{
public:
	std::vector<glm::ivec2> const& PlayerSpawnpoints() const noexcept;
	std::vector<glm::ivec2> const& EnemySpawnpoints() const noexcept;

	float TileSize() const noexcept;
	bool WallAt(glm::ivec2 const& gridPos) const;
	bool IsPath(glm::ivec2 const& gridPos) const;
	glm::ivec2 WorldToGrid(glm::vec2 const& worldPos) const;
	glm::vec2 GridToWorld(glm::ivec2 const& gridPos) const;
	glm::ivec2 IdToGrid(int idx) const;

	void Render() const override;

	GameGrid(mg::GameObject& owner, std::filesystem::path const& filePath, float tileSize);

private:
	Tile const* GetTile(glm::ivec2 const& gridPos) const;
	Tile* GetTile(glm::ivec2 const& gridPos);

	void ComputeWalkables();
	void ComputeConnections();
	void ProcessLine(std::string const& line);
	void LoadFromFile(std::filesystem::path const& filePath);

	glm::vec2 m_gridPos;
	int m_rows;
	int m_cols;
	float m_tileSize;

	std::vector<glm::ivec2> m_playerSpawns;
	std::vector<glm::ivec2> m_enemySpawns;
	std::vector<Tile> m_tiles;

	std::shared_ptr<mg::Texture2D> m_pTileSheet;
	std::shared_ptr<mg::Texture2D> m_pBackgroundTexture;
	static float constexpr m_tileTexSize{16.f};
	static int constexpr m_walkableTileY{0};
};
#endif // !GAMEGRID_H

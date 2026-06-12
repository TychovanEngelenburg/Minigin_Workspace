#ifndef GAMEGRID_H
#define GAMEGRID_H

#include <Minigin/Components/Component.h>

#include <vector>
#include <filesystem>
#include <memory>
#include <glm/vec2.hpp>

namespace mg
{
	class Texture2D;
}

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
	enum class TankSpawnType
	{
		Player,
		BasicEnemy,
		Recogniser
	};

	struct TankSpawnPoint
	{
		glm::ivec2           GridPos{};
		TankSpawnType             Type{};
		std::optional<int>   PlayerId{ std::nullopt };
	};

	std::vector<TankSpawnPoint> const& TankSpawnpoints() const noexcept;
	glm::ivec2 const& TeleporterPos() const noexcept;

	float TileSize() const noexcept;
	glm::ivec2 const& Size() const noexcept;
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

	glm::ivec2 m_gridSize{};
	float m_tileSize;

	std::vector<TankSpawnPoint> m_tankSpawns{};
	glm::ivec2 m_teleporterGridPos{};
	std::vector<Tile> m_tiles{};

	std::shared_ptr<mg::Texture2D> m_pTileSheet{};
	std::shared_ptr<mg::Texture2D> m_pBackgroundTexture{};

	static float constexpr m_tileTexSize{16.f};
	static int constexpr m_walkableTileY{0};
};
#endif // !GAMEGRID_H

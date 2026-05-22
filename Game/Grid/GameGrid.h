#ifndef GAMEGRID_H
#define GAMEGRID_H
#include "glm/vec2.hpp"
#include <vector>
#include "Minigin/EngineComponents/Component.h"
#include <filesystem>
#include <memory>
#include "Minigin/SDL_Implementation/SDLTexture2D.h"
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

namespace mg
{
	class Texture2D;
};

class GameGrid final : public mg::Component
{
public:

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

	void LoadFromFile(std::filesystem::path const& filePath);
	void ComputeWalkables();
	void ComputeConnections();

	glm::vec2 m_gridPos;
	int m_rows;
	int m_cols;
	float m_tileSize;

	std::vector<Tile> m_tiles;

	std::shared_ptr<mg::Texture2D> m_pTileSheet;
	std::shared_ptr<mg::Texture2D> m_pBackgroundTexture;
	static float constexpr m_tileTexSize{16.f};
	static int constexpr m_walkableTileY{0};
};
#endif // !GAMEGRID_H

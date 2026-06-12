#include "Sprite.h"

#include "Minigin/Rendering/ResourceManager.h"
#include "Minigin/Rendering/SDLRenderer.h"
#include "Minigin/Scene/GameObject.h"

// .h includes
#include "Minigin/Components/Component.h"
#include <memory>
#include <filesystem>

glm::vec2 mg::Sprite::Size() const noexcept
{
	return glm::vec2(m_sourceRect.w, m_sourceRect.h);
}

void mg::Sprite::SetTexture(std::filesystem::path const& filePath)
{
	m_pTexture = ResourceManager::Instance().LoadTexture(filePath);
}

void mg::Sprite::SetSprite(glm::ivec2 const& sheetPos, bool flipX, bool flipY)
{
	assert(sheetPos.x < m_spriteSheet.cols && sheetPos.y < m_spriteSheet.rows);

	m_currentSheetPos = sheetPos;

	if (sheetPos.x >= m_spriteSheet.cols)
	{
		m_currentSheetPos.x = m_spriteSheet.cols - 1;
	}

	if (sheetPos.y >= m_spriteSheet.rows)
	{
		m_currentSheetPos.y = m_spriteSheet.rows - 1;
	}

	m_sourceRect.x = m_pTexture->Size().x / m_spriteSheet.cols * m_currentSheetPos.x;
	m_sourceRect.y = m_pTexture->Size().y / m_spriteSheet.rows * m_currentSheetPos.y;

	 
	m_sourceRect.w = (m_pTexture->Size().x / m_spriteSheet.cols) * m_tileSize.x;
	m_sourceRect.h = (m_pTexture->Size().y / m_spriteSheet.rows) * m_tileSize.y;

	m_xFlipped = flipX;
	m_yFlipped = flipY;
}

void mg::Sprite::SetPivot(glm::vec2 const& pivot)
{
	m_pivot = pivot;
}

void mg::Sprite::SetTileSize(glm::ivec2 const& tiles)
{
	m_tileSize = tiles;

	SetSprite(m_currentSheetPos, m_xFlipped, m_yFlipped);

}

#pragma region Game_Loop
void mg::Sprite::Render() const
{
	Renderer::Instance().RenderTexture(*m_pTexture, Object()->Transform(), m_pivot, m_sourceRect, m_xFlipped, m_yFlipped);
}
#pragma endregion Game_Loop

mg::Sprite::Sprite(GameObject& owner, SpriteSheet const& spriteSheetData)
	: Component(owner)
	, m_pTexture( ResourceManager::Instance().LoadTexture(spriteSheetData.filePath) )
	, m_spriteSheet( spriteSheetData )
{
	SetTileSize({ 1,1 });
	SetSprite(m_currentSheetPos);
}

mg::Sprite::Sprite(GameObject& owner, SpriteConfig const& config)
	: Component(owner)
	, m_pTexture(ResourceManager::Instance().LoadTexture(config.Sheet.filePath))
	, m_spriteSheet(config.Sheet)
	, m_currentSheetPos(config.SheetPos)
	, m_pivot(config.RenderPivot)
{
	SetTileSize(config.TileSize);
	SetSprite(config.SheetPos);
}
#include "Minigin/EngineComponents/Sprite.h"
#include "Minigin/ResourceManager.h"
#include "Minigin/Renderer.h"
#include "Minigin/Scene/GameObject.h"

// .h includes
#include "Minigin/EngineComponents/Component.h"
#include <memory>
#include <filesystem>

glm::vec2 const& mg::Sprite::Size()
{
	return glm::vec2(m_sourceRect.w, m_sourceRect.h);
}

void mg::Sprite::SetTexture(std::filesystem::path const& filePath)
{
	m_pTexture = ResourceManager::Instance().LoadTexture(filePath);
}

void mg::Sprite::SetSprite(glm::ivec2 sprite, bool flipX, bool flipY)
{

	if (sprite.x >= m_spriteSheet.cols)
	{
		sprite.x = m_spriteSheet.cols - 1;
	}

	if (sprite.y >= m_spriteSheet.rows)
	{
		sprite.y = m_spriteSheet.rows - 1;
	}

	m_sourceRect.x = m_pTexture->Size().x / m_spriteSheet.cols * sprite.x;
	m_sourceRect.y = m_pTexture->Size().y / m_spriteSheet.rows * sprite.y;

	m_sourceRect.w = m_pTexture->Size().x / m_spriteSheet.cols;
	m_sourceRect.h = m_pTexture->Size().y / m_spriteSheet.rows;

	m_xFlipped = flipX;
	m_yFlipped = flipY;
}

#pragma region Game_Loop
void mg::Sprite::Render() const
{
	Renderer::Instance().RenderTexture(*m_pTexture, Owner()->Transform(), m_sourceRect, m_xFlipped, m_yFlipped);
}
#pragma endregion Game_Loop

mg::Sprite::Sprite(GameObject& owner, std::filesystem::path const& filePath, SpriteSheet const& spriteSheetData)
	: Component(owner)
	, m_pTexture{ ResourceManager::Instance().LoadTexture(filePath) }
	, m_spriteSheet{ spriteSheetData }
{
	SetSprite({ 0, 0 });
}

#include "Minigin/EngineComponents/Sprite.h"
#include "Minigin/ResourceManager.h"
#include "Minigin/Renderer.h"
#include "Minigin/GameObject.h"

// .h includes
#include "Minigin/EngineComponents/Component.h"
#include <memory>
#include <filesystem>

void mg::Sprite::SetTexture(std::filesystem::path const& filePath)
{
	m_pTexture = ResourceManager::Instance().LoadTexture(filePath);
}

void mg::Sprite::SetSprite(int x, int y)
{

	if (x >= m_spriteSheet.cols)
	{
		x = m_spriteSheet.cols - 1;
	}

	if (y >= m_spriteSheet.rows)
	{
		y = m_spriteSheet.rows - 1;
	}

	m_sourceRect.x = m_pTexture->Size().x / m_spriteSheet.cols * x;
	m_sourceRect.y = m_pTexture->Size().y / m_spriteSheet.rows * y;

	m_sourceRect.w = m_pTexture->Size().x / m_spriteSheet.cols;
	m_sourceRect.h = m_pTexture->Size().y / m_spriteSheet.rows;
}

#pragma region Game_Loop
void mg::Sprite::Render() const
{
	Renderer::Instance().RenderTexture(*m_pTexture, GetOwner()->Transform(), m_sourceRect);
}
#pragma endregion Game_Loop

mg::Sprite::Sprite(GameObject& owner, std::filesystem::path const& filePath, SpriteSheet const& spriteSheetData)
	: Component(owner)
	, m_pTexture{ ResourceManager::Instance().LoadTexture(filePath) }
	, m_spriteSheet{ spriteSheetData }
	, m_sourceRect{}
{
	SetSprite(0, 0);
}

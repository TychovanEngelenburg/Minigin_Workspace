#include "EngineComponents/Sprite.h"

#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

// .h includes
#include "EngineComponents/Component.h"
#include <memory>
#include <filesystem>

void dae::Sprite::SetTexture(std::filesystem::path const& filePath)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filePath);
}

void dae::Sprite::SetSprite(int x, int y)
{

	if (x >= m_spriteSheet.cols)
	{
		x = m_spriteSheet.cols - 1;
	}

	if (y >= m_spriteSheet.rows)
	{
		y = m_spriteSheet.rows - 1;
	}

	m_sourceRect.x = m_texture->GetSize().x / m_spriteSheet.cols * x;
	m_sourceRect.y = m_texture->GetSize().y / m_spriteSheet.rows * y;

	m_sourceRect.w = m_texture->GetSize().x / m_spriteSheet.cols;
	m_sourceRect.h = m_texture->GetSize().y / m_spriteSheet.rows;
}


#pragma region Game_Loop
void dae::Sprite::Render() const
{
	SDL_FRect dst{};
	dst.x = GetOwner()->GetTransform().GetPosition().x;
	dst.y = GetOwner()->GetTransform().GetPosition().y;
	
	dst.w = m_sourceRect.w;
	dst.h = m_sourceRect.h;
	//SDL_GetTextureSize(m_texture->GetSDLTexture(), &dst.w, &dst.h);

	SDL_RenderTexture(Renderer::GetInstance().GetSDLRenderer(), m_texture->GetSDLTexture(), &m_sourceRect, &dst);
}

dae::Sprite::Sprite(GameObject& owner, std::filesystem::path const& filePath)
	: Component(owner)
	, m_texture{ ResourceManager::GetInstance().LoadTexture(filePath) }
	, m_spriteSheet{}
	, m_sourceRect{}
{
	SetSprite(0, 0);
}

dae::Sprite::Sprite(GameObject& owner, std::filesystem::path const& filePath, SpriteSheet const& spriteSheetData)
	: Component(owner)
	, m_texture{ ResourceManager::GetInstance().LoadTexture(filePath) }
	, m_spriteSheet{ spriteSheetData }
	, m_sourceRect{}
{
	SetSprite(0, 0);
}
#pragma endregion Game_Loop

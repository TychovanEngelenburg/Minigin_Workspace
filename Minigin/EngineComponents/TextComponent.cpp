#include "Minigin/EngineComponents/TextComponent.h"
#include "Minigin/Scene/GameObject.h"
#include "Minigin/Renderer.h"
#include "Minigin/ResourceManager.h"

#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_render.h>

// .h includes
#include "Minigin/EngineComponents/Component.h"
#include "Minigin/SDL_Implementation/SDLFont.h"
#include "Minigin/SDL_Implementation/SDLTexture2D.h"

#include <string>
#include <string_view>
#include <memory>
#include <SDL3/SDL_pixels.h>
#include <filesystem>
#include <cstdint>

void mg::TextComponent::SetText(std::string_view text)
{
	m_text = text;
	m_needsUpdate = true;
}

void mg::TextComponent::SetColor(SDL_Color const& color)
{
	m_color = color;
	m_needsUpdate = true;
}

#pragma region Game_loop
void mg::TextComponent::Update()
{
	if (m_needsUpdate)
	{
		auto const surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_text.c_str(), m_text.length(), m_color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}

		auto texture = SDL_CreateTextureFromSurface(Renderer::Instance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}

		SDL_DestroySurface(surf);
		m_pTextTexture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void mg::TextComponent::Render() const
{
	if (m_pTextTexture)
	{
		Renderer::Instance().RenderTexture(*m_pTextTexture, Owner()->Transform());
	}
}
std::string_view mg::TextComponent::GetText() const noexcept
{
	return m_text;
}
glm::vec2 mg::TextComponent::Size() const
{
	if (m_pTextTexture)
	{
		return 	m_pTextTexture->Size();
	}
	else
	{
		return glm::vec2();
	}
}
#pragma endregion Game_loop


mg::TextComponent::TextComponent(mg::GameObject& owner, std::string_view text, std::filesystem::path const& fontFile, uint8_t size, SDL_Color const& color)
	: Component(owner)
	, m_needsUpdate(true)
	, m_text(text)
	, m_color(color)
	, m_pFont(mg::ResourceManager::Instance().LoadFont(fontFile, size))
	, m_pTextTexture(nullptr)
{

}

#include "SDLTexture2D.h"

#include "Minigin/Rendering/SDLRenderer.h"

// TODO: Use service locator to not be SDL dependant by default
#include <SDL3/SDL_render.h>

#include <stdexcept>

glm::vec2 mg::Texture2D::Size() const
{
	glm::vec2 size{};
	SDL_GetTextureSize(m_pTexture, &size.x, &size.y);
	return size;
}

SDL_Texture* mg::Texture2D::GetSDLTexture() const noexcept
{
	return m_pTexture;
}

mg::Texture2D::Texture2D(std::filesystem::path const& filePath)
{
	auto pathStr{ filePath.string() };
	SDL_Surface* surface = SDL_LoadPNG(pathStr.c_str());

	if (!surface)
	{
		throw std::runtime_error( std::string("Failed to load PNG: ") + SDL_GetError());
	}

	m_pTexture = SDL_CreateTextureFromSurface(Renderer::Instance().GetSDLRenderer(), surface);

	SDL_DestroySurface(surface);

	if (!m_pTexture)
	{
		throw std::runtime_error( std::string("Failed to create texture from surface: ") + SDL_GetError() );
	}
}

mg::Texture2D::Texture2D(SDL_Texture& texture)
	: m_pTexture(&texture)
{
}


mg::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

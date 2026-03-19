#include "Types/Texture2D.h"
#include "Renderer.h"

#include <stdexcept>
#include <SDL3/SDL_render.h>

// .h includes
#include <filesystem>
#include <glm/ext/vector_float2.hpp>
#include <glm/fwd.hpp>

glm::vec2 mg::Texture2D::GetSize() const
{
	float w{}, h{};
	SDL_GetTextureSize(m_pTexture, &w, &h);
	return { w, h };
}

SDL_Texture* mg::Texture2D::GetSDLTexture() const noexcept
{
	return m_pTexture;
}

mg::Texture2D::Texture2D(std::filesystem::path const& filePath)
	: m_pTexture{}
{
	auto pathStr{ filePath.string() };
	SDL_Surface* surface = SDL_LoadPNG(pathStr.c_str());

	if (!surface)
	{
		throw std::runtime_error(
			std::string("Failed to load PNG: ") + SDL_GetError()
		);
	}

	m_pTexture = SDL_CreateTextureFromSurface(
		Renderer::GetInstance().GetSDLRenderer(),
		surface
	);

	SDL_DestroySurface(surface);

	if (!m_pTexture)
	{
		throw std::runtime_error(
			std::string("Failed to create texture from surface: ") + SDL_GetError()
		);
	}
}

mg::Texture2D::Texture2D(SDL_Texture* texture) 
	: m_pTexture{ texture }
{
	assert(m_pTexture != nullptr);
}


mg::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

#include "SDLFont.h"

// TODO: Use service locator to not be SDL dependant by default
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_error.h>

#include <stdexcept>
#include <string>

TTF_Font* mg::Font::GetFont() const noexcept {
	return m_pFont;
}

mg::Font::Font(std::filesystem::path const& fullPath, float size) : m_pFont(nullptr)
{
	auto pathString{ fullPath.string()};

	m_pFont = TTF_OpenFont(pathString.c_str(), size);

	if (!m_pFont) 
	{
		throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
	}
}

mg::Font::~Font()
{
	TTF_CloseFont(m_pFont);
}

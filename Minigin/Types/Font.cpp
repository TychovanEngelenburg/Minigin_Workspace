#include "Types/Font.h"

#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <SDL3/SDL_error.h>

// .h includes
#include <filesystem>

TTF_Font* mg::Font::GetFont() const noexcept {
	return m_pFont;
}

mg::Font::Font(std::filesystem::path const& fullPath, float size) : m_pFont(nullptr)
{
	auto pathString{ fullPath.string()};
	m_pFont = TTF_OpenFont(pathString.c_str(), size);
	if (m_pFont == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
	}
}

mg::Font::~Font()
{
	TTF_CloseFont(m_pFont);
}

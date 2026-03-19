#include "ResourceManager.h"

#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>

// .h includes
#include "Types/Font.h"
#include "Types/Texture2D.h"

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <cstdint>
#include <utility>


std::shared_ptr<mg::Texture2D> mg::ResourceManager::LoadTexture(std::filesystem::path const& file)
{
	auto const fullPath = m_dataPath / file;
	auto const filename = std::filesystem::path(fullPath).filename().string();

	if (m_pLoadedTextures.find(filename) == m_pLoadedTextures.end())
	{
		auto texture{ std::make_shared<Texture2D>(fullPath) };
		m_pLoadedTextures.insert(std::pair(filename, std::move(texture)));
	}
	return m_pLoadedTextures.at(filename);
}

std::shared_ptr<mg::Font> mg::ResourceManager::LoadFont(std::filesystem::path const& file, uint8_t size)
{
	auto const fullPath = m_dataPath / file;
	auto const filename = std::filesystem::path(fullPath).filename().string();

	auto const key = std::pair<std::string, uint8_t>(filename, size);

	if (m_pLoadedFonts.find(key) == m_pLoadedFonts.end())
	{
		auto font{ std::make_shared<Font>(fullPath, size) };
		m_pLoadedFonts.insert(std::pair(key, std::move(font)));
	}
	return m_pLoadedFonts.at(key);
}

void mg::ResourceManager::UnloadUnusedResources()
{
	for (auto it = m_pLoadedTextures.begin(); it != m_pLoadedTextures.end();)
	{
		if (it->second.use_count() <= 1)
		{
			it = m_pLoadedTextures.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = m_pLoadedFonts.begin(); it != m_pLoadedFonts.end();)
	{
		if (it->second.use_count() <= 1)
		{
			it = m_pLoadedFonts.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void mg::ResourceManager::Init(std::filesystem::path const& dataPath)
{
	m_dataPath = dataPath;

	if (!TTF_Init())
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

mg::ResourceManager::~ResourceManager()
{
	m_pLoadedFonts.clear();
	m_pLoadedTextures.clear();

	TTF_Quit();
}

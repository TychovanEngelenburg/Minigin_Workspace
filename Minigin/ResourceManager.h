#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Singleton.h"

#include "Types/Font.h"
#include "Types/Texture2D.h"

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <cstdint>
#include <utility>

namespace mg
{
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		std::shared_ptr<Texture2D> LoadTexture(std::filesystem::path const& file);
		std::shared_ptr<Font> LoadFont(std::filesystem::path const& file, uint8_t size);

		void UnloadUnusedResources();

		void Init(const std::filesystem::path& data);
	private:
		friend class Singleton<ResourceManager>;
		std::filesystem::path m_dataPath;

		std::map<std::string, std::shared_ptr<Texture2D>> m_pLoadedTextures;
		std::map<std::pair<std::string, uint8_t>, std::shared_ptr<Font>> m_pLoadedFonts;

		ResourceManager() = default;
		~ResourceManager();
	};
}
#endif // !RESOURCEMANAGER_H

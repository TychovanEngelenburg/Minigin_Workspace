#ifndef SPRITE_CONFIG_H
#define SPRITE_CONFIG_H

#include <filesystem>
#include <glm/vec2.hpp>
namespace mg
{
	struct SpriteSheet
	{
		std::filesystem::path filePath;

		int cols{ 1 };
		int rows{ 1 };
	};

	struct SpriteConfig
	{
		SpriteSheet Sheet{};
		glm::ivec2 SheetPos{};

		glm::vec2 RenderPivot{ 0.f, 0.f };
	};
}

#endif // !SPRITE_CONFIG_H
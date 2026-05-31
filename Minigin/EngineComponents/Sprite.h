#ifndef SPRITE_H
#define SPRITE_H

#include "Minigin/EngineComponents/Component.h"
#include "Minigin/SDL_Implementation/SDLTexture2D.h"

#include <memory>
#include <filesystem>
#include <SDL3/SDL_rect.h>


// TODO: Replace with sprite sheet system as well as utilizing a rendercomponent.


namespace mg
{
	struct SpriteSheet
	{
		int cols = 1;
		int rows = 1;
	};

	class GameObject;
	class Sprite final : public Component
	{
	public:
		glm::vec2 const& Size();

		void SetTexture(std::filesystem::path const& filename);
		void SetSprite(glm::ivec2 sprite, bool flipX = false, bool flipY = false);

		void Render() const override;

		Sprite(GameObject& owner, std::filesystem::path const& filePath, SpriteSheet const& spriteSheetData = {});

	private:
		std::shared_ptr<Texture2D> m_pTexture;
		SpriteSheet  m_spriteSheet;
		SDL_FRect m_sourceRect{};
		bool m_xFlipped{ false };
		bool m_yFlipped{ false };
	};
}
#endif // !SPRITE_H

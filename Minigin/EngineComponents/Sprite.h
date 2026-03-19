#ifndef SPRITE_H
#define SPRITE_H

#include "EngineComponents/Component.h"
#include "Types/Texture2D.h"

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
		void SetTexture(std::filesystem::path const& filename);
		void SetSprite(int x, int y);

		void Render() const override;

		Sprite(GameObject& owner, std::filesystem::path const& filePath, SpriteSheet const& spriteSheetData = {});

		~Sprite() override = default;
		Sprite(Sprite const& other) = delete;
		Sprite(Sprite&& other) = delete;
		Sprite& operator=(Sprite const& other) = delete;
		Sprite& operator=(Sprite&& other) = delete;

	private:
		std::shared_ptr<Texture2D> m_pTexture;
		SpriteSheet  m_spriteSheet;
		SDL_FRect m_sourceRect;

	};
}
#endif // !SPRITE_H

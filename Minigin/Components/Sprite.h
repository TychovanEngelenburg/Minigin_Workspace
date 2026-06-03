#ifndef SPRITE_H
#define SPRITE_H

#include "Minigin/Components/Component.h"
#include "Minigin/Rendering/SDLTexture2D.h"
#include "Minigin/Rendering/SpriteConfig.h"

#include <memory>
#include <filesystem>
#include <Minigin/Rendering/SourceRect.h>
// TODO: Replace with sprite sheet system as well as utilizing a rendercomponent.

namespace mg
{
	class GameObject;
	class Sprite final : public Component
	{
	public:
		glm::vec2 Size() const noexcept;

		void SetTexture(std::filesystem::path const& filename);
		void SetSprite(glm::ivec2 const& sprite, bool flipX = false, bool flipY = false);
		void SetPivot(glm::vec2 const& pivot);

		void Render() const override;

		explicit Sprite(GameObject& owner, SpriteSheet const& sheet);
		explicit Sprite(GameObject& owner, SpriteConfig const& config);

		virtual ~Sprite() = default;
		Sprite(Sprite const& other) = delete;
		Sprite(Sprite&& other) = delete;
		Sprite& operator=(Sprite const& other) = delete;
		Sprite& operator=(Sprite&& other) = delete;

	private:
		std::shared_ptr<Texture2D> m_pTexture;
		SpriteSheet const m_spriteSheet;
		SourceRect m_sourceRect;
		glm::ivec2 m_currentSheetPos{0, 0};
		glm::vec2 m_pivot{};
		bool m_xFlipped{ false };
		bool m_yFlipped{ false };
	};
}
#endif // !SPRITE_H

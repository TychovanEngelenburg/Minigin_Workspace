#ifndef TEXTUREOBJECT_H
#define TEXTUREOBJECT_H

#include "Minigin/EngineComponents/Component.h"
#include "Minigin/SDL_Implementation/SDLFont.h"
#include "Minigin/SDL_Implementation/SDLTexture2D.h"

#include <string>
#include <string_view>
#include <memory>
#include <SDL3/SDL_pixels.h>
#include <cstdint>
#include <filesystem>
#include <glm/glm.hpp>

namespace mg
{
	class GameObject;
	class TextComponent final : public Component
	{
	public:
		std::string_view GetText() const noexcept;
		glm::vec2 Size() const;

		void SetText(std::string_view text);
		void SetColor(SDL_Color const& color);
	
		void Update() override;
		void Render() const override;

		TextComponent(mg::GameObject& owner, std::string_view text, std::filesystem::path const& fontFile, uint8_t size, SDL_Color const& color = { 255, 255, 255, 255 });

	private:
		bool m_needsUpdate;
		std::string m_text;
		
		SDL_Color m_color;

		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_pTextTexture;
	};
}
#endif // !TEXTUREOBJECT_H

#ifndef TEXTUREOBJECT_H
#define TEXTUREOBJECT_H

#include "Minigin/Components/Component.h"
#include "Minigin/Rendering/SDLFont.h"
#include "Minigin/Rendering/SDLTexture2D.h"

#include <string>
#include <string_view>
#include <memory>
#include <SDL3/SDL_pixels.h>
#include <cstdint>
#include <filesystem>
#include <glm/vec2.hpp>

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

		TextComponent(mg::GameObject& owner, std::filesystem::path const& fontFile, uint8_t size);

		virtual ~TextComponent() = default;
		TextComponent(TextComponent const& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(TextComponent const& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

	private:
		bool m_needsUpdate{ true };
		std::string m_text{"Hello, World!"};

		SDL_Color m_color{ 255, 255, 255, 255 };

		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_pTextTexture{ nullptr };
	};
}
#endif // !TEXTUREOBJECT_H

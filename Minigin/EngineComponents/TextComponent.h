#ifndef TEXTUREOBJECT_H
#define TEXTUREOBJECT_H

#include "EngineComponents/Component.h"
#include "Types/Font.h"
#include "Types/Texture2D.h"

#include <string>
#include <string_view>
#include <memory>
#include <SDL3/SDL_pixels.h>
#include <cstdint>
#include <filesystem>

namespace dae
{
	class GameObject;
	class TextComponent final : public Component
	{
	public:
		std::string_view GetText() const noexcept;

		void SetText(std::string_view text);
		void SetColor(SDL_Color const& color);
	
		void Update() override;
		void Render() const override;

		TextComponent(GameObject& owner, std::string_view text, std::filesystem::path const& fontFile, uint8_t size, SDL_Color const& color = { 255, 255, 255, 255 });
		
		~TextComponent() override = default;
		TextComponent(TextComponent const& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(TextComponent const& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

	private:
		bool m_needsUpdate{};
		std::string m_text{};
		
		SDL_Color m_color{ 255, 255, 255, 255 };

		std::shared_ptr<Font> m_font{};
		std::shared_ptr<Texture2D> m_textTexture{};
	};
}
#endif // !TEXTUREOBJECT_H

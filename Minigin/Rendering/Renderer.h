#ifndef RENDERER_H
#define RENDERER_H

#include "Minigin/Core/Singleton.h"
#include "Minigin/Rendering/SourceRect.h"

#include <glm/vec2.hpp>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

// TODO: look into making a different renderer?
namespace mg
{
	class Transform2D;
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		const SDL_Color& BackgroundColor() const;
		SDL_Renderer* GetSDLRenderer() const noexcept;

		void DrawRect(SourceRect const& rect);
		void RenderTexture(Texture2D const& texture, Transform2D const& transform, glm::vec2 const& pivot = { 0.f, 0.f }, SourceRect const& src = SourceRect{}, bool flipX = false, bool flipY = false) const;
		void RenderTexture(Texture2D const& texture, SourceRect const& dst, SourceRect const& src = SourceRect{}, bool flipX = false, bool flipY = false) const;
		void SetBackgroundColor(SDL_Color const& color);

		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

	private:
		SDL_Renderer* m_pRenderer{};
		SDL_Window* m_pWindow{};
		SDL_Color m_clearColor{};
	};
}
#endif // !RENDERER_H

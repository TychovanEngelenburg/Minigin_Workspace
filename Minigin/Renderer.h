#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "Singleton.h"
#include <memory>

// TODO: look into making a different renderer?
namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		const SDL_Color& GetBackgroundColor() const;
		SDL_Renderer* GetSDLRenderer() const noexcept;

		void RenderTexture(Texture2D const& texture, float x, float y) const;
		void RenderTexture(Texture2D const& texture, float x, float y, float width, float height) const;
		void SetBackgroundColor(SDL_Color const& color);

		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

	private:
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};
	};
}
#endif // !RENDERER_H

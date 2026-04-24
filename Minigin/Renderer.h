#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "Singleton.h"

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

		void RenderTexture(Texture2D const& texture, Transform2D const& transform, SDL_FRect const& src = {}, bool flipX = false, bool flipY = false) const;
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

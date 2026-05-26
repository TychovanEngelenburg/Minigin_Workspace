#include "Minigin/Renderer.h"
#include "Minigin/Scene/SceneManager.h"
#include "Minigin/SDL_Implementation/SDLTexture2D.h"

#include <SDL3/SDL_video.h>
#include <stdexcept>
#include <iostream>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include "Singleton.h"

SDL_Renderer* mg::Renderer::GetSDLRenderer() const noexcept
{
	return m_pRenderer;
}

void mg::Renderer::DrawRect(SDL_FRect const& rect)
{
	SDL_SetRenderDrawColor(Renderer::Instance().GetSDLRenderer(), 255, 0, 0, 255);
	SDL_RenderRect(Renderer::Instance().GetSDLRenderer(), &rect);
}

void mg::Renderer::RenderTexture(Texture2D const& texture, Transform2D const& transform, SDL_FRect const& src, bool flipX, bool flipY) const
{
	//assert((src.h > 0) ^ (src.w > 0));
	//assert(src.x > 0 && src.y > 0);

	bool hasSrc{ true };
	if (src.w == 0 && src.h == 0)
	{
		hasSrc = false;
	}

	SDL_FRect dst{};
	auto pos{ transform.WorldPosition() };
	dst.x = pos.x;
	dst.y = pos.y;

	auto scale{ transform.WorldScale() };

	if (hasSrc)
	{
		dst.w = src.w;
		dst.h = src.h;
	}
	else
	{
		SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
	}

	dst.w *= scale.x;
	dst.h *= scale.y;

	// Negative to comply with GLM's Y-up (Clockwise) rotation.
	auto rot = -transform.WorldRotationZ();

	//SDL_FPoint rotPivot{ dst.w * 0.5f, dst.h * 0.5f };
	SDL_FPoint rotPivot{ 0.f, 0.f };

	SDL_FlipMode flipMode{};
	if (flipX && !flipY)
	{
		flipMode = SDL_FLIP_HORIZONTAL;
	}
	else if (!flipX && flipY)
	{
		flipMode = SDL_FLIP_VERTICAL;
	}
	if (flipX && flipY)
	{
		flipMode = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
	}

	SDL_RenderTextureRotated(
		Renderer::Instance().GetSDLRenderer(),
		texture.GetSDLTexture(),
		hasSrc ? &src : nullptr,
		&dst,
		rot,
		&rotPivot,
		flipMode
	);
}

void mg::Renderer::RenderTexture(Texture2D const& texture, SDL_FRect const& dst, SDL_FRect const& src, bool flipX, bool flipY) const
{

	bool hasSrc{ true };
	if (src.w == 0 && src.h == 0)
	{
		hasSrc = false;
	}

	SDL_FRect newDst{ dst };
	//if (hasSrc)
	//{
	//	newDst.w = src.w;
	//	newDst.h = src.h;
	//}
	//else
	//{
	//	SDL_GetTextureSize(texture.GetSDLTexture(), &newDst.w, &newDst.h);
	//}

	SDL_FlipMode flipMode{};
	if (flipX && !flipY)
	{
		flipMode = SDL_FLIP_HORIZONTAL;
	}
	else if (!flipX && flipY)
	{
		flipMode = SDL_FLIP_VERTICAL;
	}
	if (flipX && flipY)
	{
		flipMode = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
	}

	SDL_RenderTexture(
		Renderer::Instance().GetSDLRenderer(),
		texture.GetSDLTexture(),
		hasSrc ? &src : nullptr,
		&newDst
	);
}

const SDL_Color& mg::Renderer::BackgroundColor() const
{
	return m_clearColor;
}

void mg::Renderer::SetBackgroundColor(SDL_Color const& color)
{
	m_clearColor = color;
}

void mg::Renderer::Init(SDL_Window* window)
{
	m_pWindow = window;

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

#if defined(__EMSCRIPTEN__)
	m_pRenderer = SDL_CreateRenderer(window, nullptr);
#else
	m_pRenderer = SDL_CreateRenderer(window, nullptr);
#endif

	if (m_pRenderer == nullptr)
	{
		std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void mg::Renderer::Render() const
{

	auto const& color = BackgroundColor();
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_pRenderer);

	SceneManager::Instance().Render();

	SDL_RenderPresent(m_pRenderer);
}

void mg::Renderer::Destroy()
{
	if (m_pRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}
}

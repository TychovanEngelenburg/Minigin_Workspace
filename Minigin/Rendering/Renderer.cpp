#include "Renderer.h"

#include "Minigin/Scene/SceneManager.h"
#include "Minigin/Rendering/SDLTexture2D.h"
#include "Minigin/Core/Transform2D.h"

#include <SDL3/SDL_video.h>
#include <stdexcept>
#include <iostream>
#include <SDL3/SDL_hints.h>

SDL_Renderer* mg::Renderer::GetSDLRenderer() const noexcept
{
	return m_pRenderer;
}

void mg::Renderer::DrawRect(SourceRect const& rect)
{
	SDL_FRect sdlDest{ rect.x, rect.y, rect.w, rect.h };

	SDL_SetRenderDrawColor(Renderer::Instance().GetSDLRenderer(), 255, 0, 0, 255);
	SDL_RenderRect(Renderer::Instance().GetSDLRenderer(), &sdlDest);
}

void mg::Renderer::RenderTexture(Texture2D const& texture, Transform2D const& transform, glm::vec2 const& pivot, SourceRect const& src, bool flipX, bool flipY) const
{

	//assert((src.h > 0) ^ (src.w > 0));
	//assert(src.x > 0 && src.y > 0);

	bool hasSrc{ !(src.w == 0 && src.h == 0) };


	SDL_FRect dst{};
	auto pos{ transform.WorldPosition() };

	dst.w = hasSrc ? src.w : 0;
	dst.h = hasSrc ? src.h : 0;

	if (!hasSrc)
	{
		SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
	}

	auto scale{ transform.WorldScale() };
	dst.w *= scale.x;
	dst.h *= scale.y;

	glm::vec2 scaledPivot = { pivot.x * scale.x, pivot.y * scale.y };
	dst.x = pos.x - scaledPivot.x;
	dst.y = pos.y - scaledPivot.y;

	auto rot = transform.WorldRotationZ();

	SDL_FPoint rotPivot{ pivot.x, pivot.y };

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

	SDL_FRect sdlSource{ src.x, src.y, src.w, src.h };
	SDL_RenderTextureRotated(
		Renderer::Instance().GetSDLRenderer(),
		texture.GetSDLTexture(),
		hasSrc ? &sdlSource : nullptr,
		&dst,
		rot,
		&rotPivot,
		flipMode
	);
}

void mg::Renderer::RenderTexture(Texture2D const& texture, SourceRect const& dst, SourceRect const& src, bool flipX, bool flipY) const
{
	bool hasSrc{ !(src.w == 0 && src.h == 0) };


	SDL_FRect sdlSource{ src.x, src.y, src.w, src.h };
	SDL_FRect newDst{ dst.x, dst.y, dst.w, dst.h };

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
		hasSrc ? &sdlSource : nullptr,
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

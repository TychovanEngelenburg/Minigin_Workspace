#include "Minigin/Renderer.h"
#include "Minigin/SceneManager.h"
#include "Minigin/SDL_Implementation/SDLTexture2D.h"

#include <SDL3/SDL_video.h>
#include <stdexcept>
#include <iostream>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include "Singleton.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

#include "Minigin/ImGUI/imgui_plot.h"

SDL_Renderer* mg::Renderer::GetSDLRenderer() const noexcept
{
	return m_pRenderer;
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


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
#if __EMSCRIPTEN__
	// For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
	// You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
	io.IniFilename = NULL;
#endif

	ImGui_ImplSDL3_InitForSDLRenderer(window, m_pRenderer);
	ImGui_ImplSDLRenderer3_Init(m_pRenderer);

}

void mg::Renderer::Render() const
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();


	auto const& color = BackgroundColor();
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_pRenderer);

	SceneManager::Instance().Render();
	ImGui::Render();

	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_pRenderer);

	SDL_RenderPresent(m_pRenderer);
}

void mg::Renderer::Destroy()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	if (m_pRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}
}

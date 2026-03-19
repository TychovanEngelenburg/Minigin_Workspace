#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <filesystem>

#include <glm/ext/vector_float2.hpp>
#include <glm/fwd.hpp>

struct SDL_Texture;
namespace mg
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D final
	{
	public:
		SDL_Texture* GetSDLTexture() const noexcept;
		glm::vec2 GetSize() const;

		explicit Texture2D(SDL_Texture* texture);
		explicit Texture2D(std::filesystem::path const& fullPath);

		~Texture2D();
		Texture2D(Texture2D const&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator= (Texture2D const&) = delete;
		Texture2D& operator= (Texture2D&&) = delete;

	private:
		SDL_Texture* m_pTexture;
	};
}
#endif // !TEXTURE2D_H

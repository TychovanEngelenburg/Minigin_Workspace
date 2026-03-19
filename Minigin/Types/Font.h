#ifndef FONT_H
#define FONT_H

#include <filesystem>

struct TTF_Font;

namespace mg
{
	/**
	 * Simple RAII wrapper for a TTF_Font
	 */
	class Font final
	{
	public:
		TTF_Font* GetFont() const noexcept;

		explicit Font(std::filesystem::path const& fullPath, float size);

		~Font();
		Font(Font const&) = delete;
		Font(Font&&) = delete;
		Font& operator= (Font const&) = delete;
		Font& operator= (Font const&&) = delete;

	private:
		TTF_Font* m_pFont;
	};
}
#endif // !FONT_H

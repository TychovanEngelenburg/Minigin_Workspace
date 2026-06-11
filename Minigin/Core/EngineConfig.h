#ifndef ENGINE_CONFIG_H
#define ENGINE_CONFIG_H

#include <string>
#include <filesystem>
namespace mg
{
	struct WindowConfig
	{
		std::string title{ "Minigin" };
		int width{ 1280 };
		int height{ 720 };

		// bool fullscreen{ false };		Not supported yet!
		// bool resizable{ true };			Not supported yet!
		// bool vsync{ true };				Not supported yet!
	};

	struct EngineConfig
	{
		WindowConfig window{};
		std::filesystem::path assetPath{"./Data/"};
	};
}
#endif // !ENGINE_CONFIG_H
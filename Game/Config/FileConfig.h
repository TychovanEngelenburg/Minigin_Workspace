#ifndef FILE_LOCATIONS_H
#define FILE_LOCATIONS_H

#include "Game/Core/GameModes.h"
#include <Minigin/Rendering/SpriteConfig.h>
#include <filesystem>
#include <vector>

// Locations are relative to .Data/


// TODO: Move all of this to a separate file.

namespace Files
{
	// Fonts
	inline  std::filesystem::path const JoystixFont{ "joystixmonospace-regular.otf" };

	// Textures/Sprites
	inline  mg::SpriteSheet const EntitySpriteSheet{ "T_SpriteSheet_BattleTanks.png" , 4, 4 };
	inline  mg::SpriteSheet const GridTileSheet{ "T_TileSheet_Tron_BattleTanks.png" , 4, 1 };
	inline  std::filesystem::path const GridBackground{ "T_Motherboard_Background.png" };

	// Audio (TODO: Make these not haveto include the ./Data/ path)
	inline  std::filesystem::path const GameMusic{"./Data/Audio_Tron1982/03_IO_Tower.wav" };
	inline  std::filesystem::path const TankDamaged{ "./Data/Audio_Tron1982/09_Sounds_MCP_Blip.wav"};
	inline  std::filesystem::path const TankDeath{ "./Data/Audio_Tron1982/11_Sounds_Derezzed.wav"};
	inline  std::filesystem::path const FailureSFX{"./Data/Audio_Tron1982/05_Fail_Music.wav" };
	inline  std::filesystem::path const WinSFX{"./Data/Audio_Tron1982/04_Success_Music_(Alt).wav" };
	inline  std::filesystem::path const StartSFX{"./Data/Audio_Tron1982/01_Insert_Coin.wav" };


	// Custom
	inline  std::filesystem::path const ScoreFile{"Scores/highscores.dat"};


}

namespace LevelFiles
{

	inline const std::vector<std::filesystem::path> Singleplayer =
	{
		{"LevelData/SinglePlayer/01.lvl"},
		{"LevelData/SinglePlayer/02.lvl"},
		{"LevelData/SinglePlayer/03.lvl"}
	};

	inline const std::vector<std::filesystem::path> Coop =
	{
		{"LevelData/Coop/01.lvl"  }
		//{"LevelData/Coop/02.lvl"},
		//{"LevelData/Coop/03.lvl"}
	};

	inline const std::vector<std::filesystem::path> Versus =
	{
		{"LevelData/Versus/01.lvl" }
	};
}

#endif // ! FILE_LOCATIONS_H
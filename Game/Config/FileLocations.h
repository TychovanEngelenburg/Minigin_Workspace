#ifndef FILE_LOCATIONS_H
#define FILE_LOCATIONS_H

#include "Game/Core/GameModes.h"

#include <filesystem>
#include <vector>

// Locations are relative to .Data/


// TODO: Move all of this to a separate file.

namespace FileLocations
{
	inline  std::filesystem::path const JoystixFont{ "joystixmonospace-regular.otf" };
	inline  std::filesystem::path const TankSheet{ "T_SpriteSheet_BattleTanks.png" };
	inline  std::filesystem::path const BulletSheet{ "T_SpriteSheet_BattleTanks.png" };
	inline  std::filesystem::path const GridTileSheet{  "T_TileSheet_Tron_BattleTanks.png"  };
	inline  std::filesystem::path const GridBackground{ "T_Motherboard_Background.png" };
	inline  std::filesystem::path const ScoreFile{"Scores/highscores.dat"};
}

namespace LevelFiles
{

	inline const std::vector<std::filesystem::path> Singleplayer =
	{
		"LevelData/SinglePlayer/01.lvl",
		"LevelData/SinglePlayer/02.lvl",
		"LevelData/SinglePlayer/03.lvl"
	};

	inline const std::vector<std::filesystem::path> Coop =
	{
		"LevelData/Coop/01.lvl",
		"LevelData/Coop/02.lvl",
		"LevelData/Coop/03.lvl"
	};

	inline const std::vector<std::filesystem::path> Versus =
	{
		"LevelData/Versus/01.lvl"
	};
}

#endif // ! FILE_LOCATIONS_H
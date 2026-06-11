#ifndef FILE_LOCATIONS_H
#define FILE_LOCATIONS_H

#include <filesystem>
// Locations relative to .Data/

namespace FileLocations
{
	inline  std::filesystem::path const JoystixFont{ "joystixmonospace-regular.otf" };
	inline  std::filesystem::path const TankSheet{ "T_SpriteSheet_BattleTanks.png" };
	inline  std::filesystem::path const BulletSheet{ "T_SpriteSheet_BattleTanks.png" };
	inline  std::filesystem::path const ScoreFile{"Scores/highscores.dat"};
}

#endif // ! FILE_LOCATIONS_H
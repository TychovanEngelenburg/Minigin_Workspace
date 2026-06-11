#ifndef SCENE_LOADING_H
#define SCENE_LOADING_H

#include "Game/Core/GameContext.h"
#include "Game/Components/System/TankManager.h"
#include <filesystem>

namespace mg
{
	class Scene;
}

namespace SceneLoading
{
	TankManager::SpawnCounts LoadLevelScene( mg::Scene& scene, std::filesystem::path const&, GameMode mode);

	void LoadMainMenuScene(mg::Scene& sceneOut);
	void LoadScoreSavingScene(mg::Scene& sceneOut);
	void LoadScoreboardScene(mg::Scene& sceneOut);
};
#endif //SCENE_LOADING_H

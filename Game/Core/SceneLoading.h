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
	void LoadTestScene(mg::Scene& sceneOut);

	TankManager::SpawnCounts LoadLevelScene( mg::Scene& scene, std::filesystem::path const&, GameContext::GameMode mode);

	void LoadMainMenuScene(mg::Scene& sceneOut);
	void LoadScoreboardScene(mg::Scene& sceneOut);
};
#endif //SCENE_LOADING_H

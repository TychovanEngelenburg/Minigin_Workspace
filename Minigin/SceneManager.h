#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Singleton.h"
#include "Minigin/Scene.h"

#include <memory>
#include <vector>

namespace mg
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene* GetScene(size_t sceneId);

		size_t CreateScene();
		Scene const& CreateSceneAt(size_t sceneId);
		Scene* SetActiveScene(size_t sceneId);

		void Start();
		void FixedUpdate();
		void ProcessInput();
		void Update();
		void Render() const;
		void LateUpdate();
		void End();

		SceneManager();
		virtual ~SceneManager() = default;
		SceneManager(SceneManager const& other) = delete;
		SceneManager(SceneManager&& other) = delete;
		SceneManager& operator=(SceneManager const& other) = delete;
		SceneManager& operator=(SceneManager&& other) = delete;

	private:
		friend class Singleton<SceneManager>;

		std::vector<std::unique_ptr<Scene>> m_pScenes;

		size_t m_activeScene;
	};
}
#endif // !SCENEMANAGER_H

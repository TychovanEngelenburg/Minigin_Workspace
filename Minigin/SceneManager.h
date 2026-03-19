#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Singleton.h"
#include "Scene.h"

#include <memory>
#include <vector>

namespace mg
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene();

		void Start();
		void FixedUpdate();
		void Update();
		void Render() const;
		void LateUpdate();
		void End();

	private:
		friend class Singleton<SceneManager>;

		SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_pScenes;
	};
}
#endif // !SCENEMANAGER_H

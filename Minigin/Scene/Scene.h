#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include <string_view>

namespace mg
{
	class SceneInput;
	class SceneCollisions;
	class GameObject;
	class Scene final
	{
	public:
		SceneInput& InputSystem() const noexcept;

		SceneCollisions& CollisionSystem() const noexcept;

		/// <summary>
		/// Quick and dirty way to get the first GameObject with a specific string name. 
		/// Try to prevent use; heavy string comparison and no collision prevention.
		/// </summary>
		GameObject* GetObjectByName(std::string_view objName);

		void Add(std::unique_ptr<GameObject> object);

		void FixedUpdate();
		void ProcessInput();
		void Update();
		void LateUpdate();
		void Render() const;
		void Cleanup();

		Scene();

		~Scene();
		Scene(Scene const& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(Scene const& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		std::unique_ptr<SceneInput> m_pInputSystem;
		std::unique_ptr<SceneCollisions> m_pCollisionSystem;
		std::vector<std::unique_ptr<GameObject>> m_pObjects{};
	};
}
#endif // !SCENE_H

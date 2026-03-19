#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include "GameObject.h"
#include <string_view>

namespace mg
{
	class Scene final
	{
	public:
		/// <summary>
		/// Quick and dirty way to get the first GameObject with a specific string name. 
		/// Try to prevent use; heavy string comparison and no collision prevention.
		/// </summary>
		GameObject* GetObjectByName(std::string_view objName);

		// TODO: Look if it would be better to implement similar to addcomponent.
		void Add(std::unique_ptr<GameObject> object);
		void Remove(GameObject const& object);

		void Start();
		void FixedUpdate();
		void Update();
		void Render() const;
		void LateUpdate();
		void End();

		~Scene();
		Scene(Scene const& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(Scene const& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		friend class SceneManager;
		explicit Scene() = default;

		std::vector<std::unique_ptr<GameObject>> m_pObjects;
	};
}
#endif // !SCENE_H

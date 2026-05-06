#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include "Minigin/GameObject.h"
#include "Minigin/InputHandling/SceneInput.h"
#include <string_view>

namespace mg
{
	class Scene final
	{
	public:
		SceneInput& Input() const;

		/// <summary>
		/// Quick and dirty way to get the first GameObject with a specific string name. 
		/// Try to prevent use; heavy string comparison and no collision prevention.
		/// </summary>
		GameObject* GetObjectByName(std::string_view objName);

		// TODO: Look if it would be better to implement similar to addcomponent.
		void Add(std::unique_ptr<GameObject> object);
		void Remove(GameObject const& object);

		void Awake();
		void Start();
		void ProcessInput();
		void FixedUpdate();
		void Update();
		void Render() const;
		void LateUpdate();
		void End();

		explicit Scene();

		~Scene();
		Scene(Scene const& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(Scene const& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		friend class SceneManager;

		std::unique_ptr<SceneInput> m_pInput;
		std::vector<std::unique_ptr<GameObject>> m_pObjects;
	};
}
#endif // !SCENE_H

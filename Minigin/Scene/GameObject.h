#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Minigin/Components/Component.h"
#include "Minigin/Core/Transform2D.h"

#include <string>
#include <string_view>
#include <memory>
#include <concepts>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>
#include <list>
#include <unordered_map>
#include <glm/vec2.hpp>

namespace mg
{
	struct CollisionData;
	class Scene;
	class GameObject final
	{
	public:
		Transform2D& Transform();
		bool IsActive() const noexcept;
		bool IsDestroyed() const noexcept;
		Scene* Scene() const noexcept;

		void SetScene(mg::Scene* pScene);
		void SetActive(bool isActive);
		void Destroy();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args);
		template<typename T>
		T* GetComponent() const;

		template<typename T>
		void RemoveComponent();

		/// <summary>
		/// Called when object is added to the scene.
		/// </summary>
		void Awake();

		/// <summary>
		/// Called when scene fully loaded.
		/// </summary>
		void Start();

		void OnCollisionEnter(CollisionData const& data);
		void OnCollisionStay(CollisionData const& data);
		void OnCollisionExit(CollisionData const& data);

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;

		explicit GameObject(std::string_view name = "New GameObject", glm::vec2 const& pos = {0.f, 0.f});
		~GameObject();

		GameObject(GameObject const& other) = delete;
		GameObject(GameObject&& other) = delete;

		GameObject& operator=(GameObject const& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		std::string Name;

	private:
		mg::Scene* m_pScene{};

		Transform2D m_transform;
		bool m_active{true};
		bool m_destroyed{};
		std::vector< std::unique_ptr<Component>> m_pComponents{};

		GameObject* m_pParent{};
		std::vector<GameObject*> m_pChildren{};
	};


	template<typename T, typename... Args>
	inline T& GameObject::AddComponent(Args&&... args)
	{
		static_assert(std::derived_from<T, Component>, "Attempted to add a non-component to component list!");

		if (GetComponent<T>())
		{
			throw std::runtime_error("Attempted to add duplicate entry to component list!");
		}

		auto component{ std::make_unique<T>(*this, std::forward<Args>(args)...) };
		auto& returnRef{ *component };
		m_pComponents.emplace_back(std::move(component));
		return returnRef;
	}

	template<typename T>
	inline T* GameObject::GetComponent() const
	{

		// Opted for dynamic cast over component.h having a GetType() option,
		// since working with std::type_index is (as far as I can tell) slower than dynamic_cast.

		for (auto& component : m_pComponents)
		{
			if (auto compPtr{ dynamic_cast<T*>(component.get()) })
			{
				return compPtr;
			}
		}

		return nullptr;
	}

	template<typename T>
	inline void GameObject::RemoveComponent()
	{
		std::erase_if(m_pComponents, [](std::unique_ptr<Component> const& component)
			{
				return dynamic_cast<T*>(component.get());
			});
	}
}
#endif // !GAMEOBJECT_H

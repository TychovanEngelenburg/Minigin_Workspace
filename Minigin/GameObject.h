#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "EngineComponents/Component.h"
#include "Types/Transform.h"

#include <string>
#include <string_view>
#include <memory>
#include <concepts>
#include <glm/fwd.hpp>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>
#include <list>
#include <unordered_map>

namespace mg
{
	class Texture2D;
	class GameObject final
	{
	public:
		Transform& GetTransform();
		std::string const& GetName() const noexcept;
		bool IsDestroyed() const noexcept;
		bool IsActive() const noexcept;

		void SetActive(bool isActive);
		//void SetPosition(float x, float y);

		void Destroy();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args);

		// Removed because functionally no improvement over GetComponent in it's current state.
		//template<typename T>
		//bool HasComponent() const;

		template<typename T>
		T* GetComponent() const;

		template<typename T>
		void RemoveComponent();

		void Start();
		void Update();
		void FixedUpdate();
		void Render() const;
		void LateUpdate();
		void End();

		GameObject(std::string_view name, glm::vec3 pos = {0.f, 0.f, 0.f});
		~GameObject();

		// TODO: Implement Copy and Move constructors
		GameObject(GameObject const& other) = delete;
		GameObject(GameObject&& other) = delete;

		GameObject& operator=(GameObject const& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform;
		std::string m_name;
		bool m_active;
		bool m_destroyed;
		std::vector< std::unique_ptr<Component>> m_pComponents;
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

	
	//template<typename T>
	//inline bool GameObject::HasComponent() const
	//{
	//	for (auto& component : m_pComponents)
	//	{
	//		if (component.GetType() == std::type_index(typeid(T)))
	//		{
	//			return true;
	//		}
	//	}

	//	return false;
	//}

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

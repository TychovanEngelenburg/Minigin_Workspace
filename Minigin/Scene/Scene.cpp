#include "Scene.h"

#include "Minigin/Scene/GameObject.h"
#include "Minigin/Input/SceneInput.h"
#include "Minigin/Collisions/CollisionSystem.h"

#include <algorithm>
#include <cassert>
#include <utility>


mg::SceneInput& mg::Scene::InputSystem() const noexcept
{
	assert(this); // Currently fails
	assert(m_pInputSystem);
	return *m_pInputSystem;
}

mg::SceneCollisions& mg::Scene::CollisionSystem() const noexcept
{
	return *m_pCollisionSystem;
}

mg::GameObject* mg::Scene::GetObjectByName(std::string_view objName)
{
	auto it = std::find_if(m_pObjects.begin(), m_pObjects.end(),
		[&objName](auto const& ptr)
		{
			return ptr->Name == objName;
		});


	if (it != m_pObjects.end())
	{
		return it->get();
	}
	return nullptr;
}

void mg::Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	object->SetScene(this);
	object->Awake();
	m_pObjects.emplace_back(std::move(object));
}

#pragma region Game_Loop
void mg::Scene::FixedUpdate()
{
	m_pCollisionSystem->Update();

	for (auto& object : m_pObjects)
	{
		if (object->ActiveInHieriarchy())
		{
			object->FixedUpdate();
		}
	}
}

void mg::Scene::ProcessInput()
{
	m_pInputSystem->ProcessInput();
}

void mg::Scene::Update()
{
	for (auto& object : m_pObjects)
	{
		if (object->ActiveInHieriarchy())
		{
			object->Update();
		}
	}
}

void mg::Scene::LateUpdate()
{

	for (auto const& object : m_pObjects)
	{
		if (object->ActiveInHieriarchy())
		{
			object->LateUpdate();
		}
	}
}

void mg::Scene::Render() const
{
	for (auto const& object : m_pObjects)
	{
		if (object->ActiveInHieriarchy())
		{
			object->Render();
		}
	}
}
void mg::Scene::Cleanup()
{
	for (auto const& object : m_pObjects)
	{
		object->Cleanup();
	}

	std::erase_if(m_pObjects, [](auto const& object)
		{
			return object->IsDestroyed();
		});
}
#pragma endregion Game_Loop

mg::Scene::Scene()
	: m_pInputSystem(std::make_unique<SceneInput>())
	, m_pCollisionSystem(std::make_unique<SceneCollisions>())
{
}

mg::Scene::~Scene() = default;

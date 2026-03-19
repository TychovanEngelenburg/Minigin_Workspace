#include "Scene.h"

#include "GameObject.h"
#include <algorithm>
#include <cassert>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>


mg::GameObject* mg::Scene::GetObjectByName(std::string_view objName)
{
	return std::find_if(m_pObjects.begin(), m_pObjects.end(), [&objName](auto const& ptr)
		{
			return ptr->GetName() == objName;
		})->get();
}

void mg::Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_pObjects.emplace_back(std::move(object));
}

void mg::Scene::Remove(GameObject const& object)
{
	m_pObjects.erase(
		std::remove_if(
			m_pObjects.begin(),
			m_pObjects.end(),
			[&object](auto const& ptr) { return ptr.get() == &object; }
		),
		m_pObjects.end()
	);
}

#pragma region Game_Loop
void mg::Scene::Start()
{
	for (auto& object : m_pObjects)
	{
		if (object->IsActive())
		{
			object->Start();
		}
	}
}

void mg::Scene::FixedUpdate()
{
	for (auto& object : m_pObjects)
	{
		if (object->IsActive())
		{
			object->FixedUpdate();
		}
	}
}

void mg::Scene::Update()
{
	for (auto& object : m_pObjects)
	{
		if (object->IsActive())
		{
			object->Update();
		}
	}
}

void mg::Scene::Render() const
{
	for (auto const& object : m_pObjects)
	{
		if (object->IsActive())
		{
			object->Render();
		}
	}
}

void mg::Scene::LateUpdate()
{

	for (auto const& object : m_pObjects)
	{
		if (object->IsActive())
		{
			object->LateUpdate();
		}


		std::erase_if(m_pObjects, [](std::unique_ptr<GameObject> const& object)
			{
				return object->IsDestroyed();
			});
	}
}

void mg::Scene::End()
{
	for (auto const& object : m_pObjects)
	{
		if (object->IsActive())
		{
			object->End();
		}

	}
}
#pragma endregion Game_Loop

mg::Scene::~Scene()
{
	m_pObjects.clear();
}

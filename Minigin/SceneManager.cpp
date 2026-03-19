#include "SceneManager.h"

// .h includes
#include "Scene.h"

#include <memory>
#include <vector>

#pragma region Game_Loop
void mg::SceneManager::Start()
{
	for (auto& scene : m_pScenes)
	{
		scene->Start();
	}
}

void mg::SceneManager::FixedUpdate()
{
	for (auto& scene : m_pScenes)
	{
		scene->FixedUpdate();
	}
}

void mg::SceneManager::Update()
{
	for(auto& scene : m_pScenes)
	{
		scene->Update();
	}
}

void mg::SceneManager::Render() const
{
	for (auto const& scene : m_pScenes)
	{
		scene->Render();
	}
}

void mg::SceneManager::LateUpdate()
{
	for (auto const& scene : m_pScenes)
	{
		scene->LateUpdate();
	}
}

void mg::SceneManager::End()
{
	for (auto const& scene : m_pScenes)
	{
		scene->End();
	}
}
#pragma endregion Game_Loop

mg::Scene& mg::SceneManager::CreateScene()
{
	// TODO: Look into why std::make_unique<Scene>() doesn't work here.
	m_pScenes.emplace_back(new Scene());
	return *m_pScenes.back();
}

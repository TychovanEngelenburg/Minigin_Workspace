#include "Minigin/Scene/SceneManager.h"

// .h includes
#include "Minigin/Scene/Scene.h"

#include <memory>
#include <vector>


mg::Scene* mg::SceneManager::GetScene(size_t sceneId)
{
	if (sceneId >= m_pScenes.size())
	{
		return nullptr;
	}
	return m_pScenes[sceneId].get();
}

mg::Scene* mg::SceneManager::SetActiveScene(size_t sceneId)
{
	assert(m_pScenes[sceneId]);

	m_activeScene = sceneId;
	auto scene = m_pScenes[m_activeScene].get();
	Start();

	return m_pScenes[m_activeScene].get();
}

size_t mg::SceneManager::CreateScene()
{
	// TODO: Look into why std::make_unique<Scene>() doesn't work here.
	m_pScenes.emplace_back(std::make_unique<Scene>());
	return m_pScenes.size() - 1;
}

/// <summary>
/// Creates a new scene at the given sceneId (if within range!) and overwrites the old.
/// </summary>
mg::Scene const& mg::SceneManager::CreateSceneAt(size_t sceneId)
{
	assert(sceneId >= m_pScenes.size());

	if (sceneId < m_pScenes.size())
	{
		m_pScenes.reserve(sceneId);
	}


	m_pScenes[sceneId] = std::make_unique<Scene>();
	return *m_pScenes[sceneId];
}

#pragma region Game_Loop
void mg::SceneManager::Start()
{
	m_pScenes[m_activeScene]->Start();

}

void mg::SceneManager::FixedUpdate()
{
	m_pScenes[m_activeScene]->FixedUpdate();

}

void mg::SceneManager::ProcessInput()
{
	m_pScenes[m_activeScene]->ProcessInput();
}

void mg::SceneManager::Update()
{
	m_pScenes[m_activeScene]->Update();

}

void mg::SceneManager::Render() const
{

	m_pScenes[m_activeScene]->Render();
}

void mg::SceneManager::LateUpdate()
{

	m_pScenes[m_activeScene]->LateUpdate();

}

void mg::SceneManager::OnApplicationQuit()
{

	m_pScenes[m_activeScene]->OnApplicationQuit();
}
mg::SceneManager::SceneManager()
	: m_pScenes{}
	, m_activeScene{ 0 }
{
}
#pragma endregion Game_Loop


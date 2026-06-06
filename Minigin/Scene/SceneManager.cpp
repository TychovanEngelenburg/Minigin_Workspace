#include "SceneManager.h"

#include "Minigin/Scene/Scene.h"
#include "Minigin/Core/DeltaClock.h"

#include <cassert>

mg::Scene* mg::SceneManager::GetScene(size_t sceneId)
{
	if (sceneId >= m_pScenes.size())
	{
		return nullptr;
	}
	return m_pScenes[sceneId].get();
}

mg::Scene* mg::SceneManager::ActiveScene() const noexcept
{
	return m_pActiveScene;
}

mg::Scene* mg::SceneManager::SetActiveScene(size_t sceneId)
{
	assert(m_pScenes[sceneId]);
	//assert(m_pScenes[sceneId]);

	if (sceneId >= m_pScenes.size() || !m_pScenes[sceneId])
	{
		return nullptr;
	}

	m_activeSceneID = sceneId;
	m_pActiveScene = m_pScenes[m_activeSceneID].get();
	return m_pScenes[m_activeSceneID].get();
}

size_t mg::SceneManager::CreateScene()
{
	m_pScenes.emplace_back(std::make_unique<Scene>());
	return m_pScenes.size() - 1;
}


mg::Scene const& mg::SceneManager::CreateSceneAt(size_t sceneId)
{
	if (sceneId >= m_pScenes.size())
	{
		m_pScenes. resize(sceneId + 1);
	}

	m_pScenes[sceneId] = std::make_unique<Scene>();
	return *m_pScenes[sceneId];
}

#pragma region Game_Loop
void mg::SceneManager::Tick()
{
	if (!m_pActiveScene)
	{
		return;
	}

	m_pActiveScene->ProcessInput();

	m_lag += DeltaClock::DeltaTime();
	while (m_lag >= DeltaClock::FixedDeltaTime())
	{
		m_pActiveScene->FixedUpdate();
		m_lag -= DeltaClock::FixedDeltaTime();
	}

	m_pActiveScene->Update();
	m_pActiveScene->LateUpdate();
	m_pActiveScene->Cleanup();
}

void mg::SceneManager::Render() const
{
	if (!m_pActiveScene)
	{
		return;
	}

	m_pActiveScene->Render();
}


mg::SceneManager::SceneManager() = default;
mg::SceneManager::~SceneManager() = default;

#pragma endregion Game_Loop


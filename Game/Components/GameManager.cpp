#include "GameManager.h"

#include "Game/Core/GameContext.h"

#include <Minigin/Scene/GameObject.h>

void GameManager::Update()
{
    GameContext::Instance().FlushEvents();
}

GameManager::GameManager(mg::GameObject& owner)
    : Component(owner)
{
}

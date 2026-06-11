#include "ContinueSceneCommand.h"

#include "Game/Core/GameContext.h"

void ContinueSceneCommand::Execute()
{
	GameContext::Instance().PushEvent(GameEvent::Continue);
}
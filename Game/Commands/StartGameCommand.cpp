#include "StartGameCommand.h"

#include "Game/Core/GameContext.h"

void StartGameCommand::Execute()
{
	GameContext::Instance().HandleEvent(GameEvent::StartPlaying);
}
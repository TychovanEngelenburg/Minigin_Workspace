#include "StartGameCommand.h"
#include "GameContext.h"

void StartGameCommand::Execute()
{
	GameContext::Instance().HandleEvent(GameEvent::StartPlaying);
}
#include "StartGameCommand.h"

#include "Game/Core/GameContext.h"

void StartGameCommand::Execute()
{
	GameContext::Instance().SetupGame(GameContext::GameMode::Singleplayer);
	GameContext::Instance().HandleGameEvent(GameEvent::StartPlaying);
}
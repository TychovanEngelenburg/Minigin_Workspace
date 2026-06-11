#include "ToggleGameModeCommand.h"

#include "Game/Core/GameContext.h"

void ToggleGameModeCommand::Execute()
{
	GameContext::Instance().ToggleGamemode();
}
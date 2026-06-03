#include "TurnBarrelCommand.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/Core/DeltaClock.h>
void TurnBarrelCommand::Execute()
{
	GetGameObject()->Transform().Rotate(m_degreesPerSec * static_cast<float>(mg::DeltaClock::DeltaTime()));
}

TurnBarrelCommand::TurnBarrelCommand(mg::GameObject& object, float degreesPerSec)
	: GameObjectCommand(object)
	, m_degreesPerSec(degreesPerSec)
{
}

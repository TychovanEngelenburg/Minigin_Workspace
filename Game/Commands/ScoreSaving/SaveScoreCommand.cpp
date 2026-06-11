#include "SaveScoreCommand.h"

#include <Minigin/Scene/GameObject.h>
#include "Game/Components/System/ScoreSaver.h"

void SaveScoreCommand::Execute()
{
	m_pMovementComp->Continue();
}

SaveScoreCommand::SaveScoreCommand(mg::GameObject& object)
	: GameObjectCommand(object)
	, m_pMovementComp(object.GetComponent<ScoreSaver>())
{}


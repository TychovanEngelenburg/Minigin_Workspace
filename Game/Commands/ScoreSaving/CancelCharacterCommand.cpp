#include "CancelCharacterCommand.h"

#include <Minigin/Scene/GameObject.h>
#include "Game/Components/System/ScoreSaver.h"

void CancelCharacter::Execute()
{
	m_pMovementComp->Back();
}

CancelCharacter::CancelCharacter(mg::GameObject& object)
	: GameObjectCommand(object)
	, m_pMovementComp(object.GetComponent<ScoreSaver>())
{}

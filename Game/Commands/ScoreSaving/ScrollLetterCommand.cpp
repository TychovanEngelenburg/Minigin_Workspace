#include "ScrolllLetterCommand.h"

#include <Minigin/Scene/GameObject.h>
#include "Game/Components/System/ScoreSaver.h"

void ScrollLetterCommand::Execute()
{
	if (m_scrollBack)
	{
		m_pMovementComp->CycleLetterUp();
	}
	else
	{
		m_pMovementComp->CycleLetterDown();
	}
}
;
ScrollLetterCommand::ScrollLetterCommand(mg::GameObject& object, bool scrollBack)
	: GameObjectCommand(object)
	, m_pMovementComp( object.GetComponent<ScoreSaver>() )
	, m_scrollBack(scrollBack)
{
}

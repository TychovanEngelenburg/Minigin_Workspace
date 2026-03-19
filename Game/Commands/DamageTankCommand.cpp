#include "DamageTankCommand.h"
#include "InputHandling/GameObjectCommand.h" 
#include "GameObject.h"
#include "Tank/TankHealth.h"

void DamageTankCommand::Execute()
{
	m_pTankHealth->Damage(m_damageAmount);
}

DamageTankCommand::DamageTankCommand(mg::GameObject* object, int damageAmnt)
	: GameObjectCommand(object)
	, m_pTankHealth{ object->GetComponent<TankHealth>() }
	, m_damageAmount{ damageAmnt }
{
}

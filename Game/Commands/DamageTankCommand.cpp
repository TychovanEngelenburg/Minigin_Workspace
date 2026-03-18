#include "DamageTankCommand.h"
#include "InputHandling/GameObjectCommand.h" 
#include "GameObject.h"
#include "Tank/TankHealth.h"

void DamageTankCommand::Execute()
{
	m_tankHealth->Damage(m_damageAmount);
}

DamageTankCommand::DamageTankCommand(dae::GameObject* object, int damageAmnt)
	: GameObjectCommand(object)
	, m_tankHealth{ object->GetComponent<TankHealth>() }
	, m_damageAmount{ damageAmnt }
{
}

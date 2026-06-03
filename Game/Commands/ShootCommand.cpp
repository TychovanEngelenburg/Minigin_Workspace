#include "ShootCommand.h"

#include "Game/Tank/TankBarrel.h"

#include <Minigin/Scene/GameObject.h>

void ShootCommand::Execute()
{
	if (!m_pBarrel->OnCooldown())
	{
		m_pBarrel->Shoot();
	}
}

ShootCommand::ShootCommand(mg::GameObject& object)
	: GameObjectCommand(object)
	, m_pBarrel(object.GetComponent<TankBarrel>())
{
}

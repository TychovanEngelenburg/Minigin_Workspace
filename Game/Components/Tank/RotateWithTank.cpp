#include "RotateWithTank.h"

#include <Minigin/Components/Sprite.h>
#include <Minigin/Scene/GameObject.h>

void RotateWithTank::SetTank(TankMovement* tank)
{
	m_pMovement = tank;
}

void RotateWithTank::Update()
{
	if (m_pMovement)
	{
		CheckDirection();
	}
}


RotateWithTank::RotateWithTank(mg::GameObject& owner)
	: Component(owner)
{

}

void RotateWithTank::CheckDirection()
{
	auto dir = m_pMovement->CurrentDirection();
	if (dir != m_currentDirection)
	{
		m_currentDirection = dir;
		SetRotation();
	}
}

void RotateWithTank::SetRotation()
{
	switch (m_currentDirection)
	{

		case TankMovement::Direction::Up:
		{
			Object()->Transform().SetWorldRotation(-90.f);
			break;
		}

		case TankMovement::Direction::Down:
		{
			Object()->Transform().SetWorldRotation(90.f);
			break;
		}

		case TankMovement::Direction::Left:
		{
			Object()->Transform().SetWorldRotation(180.f);
			break;
		}

		default:
		case TankMovement::Direction::Right:
		{
			Object()->Transform().SetWorldRotation(0.f);
			break;
		}
	}
}

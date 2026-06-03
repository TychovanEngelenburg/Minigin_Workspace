#include "TankHealth.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/Events/IEventListener.h>

bool TankHealth::IsDead() const noexcept
{
	return m_Health <= 0;
}

int TankHealth::Health() const noexcept
{ 
	return m_Health;
}

void TankHealth::Damage(int amount)
{
	m_Health -= amount;

	if (IsDead())
	{
		OnDeath();
	}

	// TODO: Remove
	DemoPlaySound();
}

void TankHealth::Kill()
{
	m_Health = 0;
	OnDeath();
}



void TankHealth::ResetHealth()
{
	m_Health = maxHealth;
}


void TankHealth::AddListener(mg::IEventListener<PlayerDeath>* listener)
{
	m_onDeath.AddListener(listener);
}

TankHealth::TankHealth(mg::GameObject& owner)
	: Component(owner)
{

}

void TankHealth::OnDeath()
{
	// TODO: Play sound
	Owner()->SetActive(false);
	m_onDeath.Notify({ m_playerId });
}

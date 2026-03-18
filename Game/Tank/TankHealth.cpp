#include "TankHealth.h"
#include "GameObject.h"
#include "EventListener.h"
//#include <exception>

bool TankHealth::IsDead() const noexcept
{
	return m_currentHealth > 0;
}

bool TankHealth::CanRespawn() const noexcept
{
	return m_lives > 0;
}

int TankHealth::GetHealth() const noexcept
{ 
	return m_currentHealth;
}

int TankHealth::GetLives() const noexcept
{
	return m_currentLives;
}

void TankHealth::Damage(int amount)
{
	m_currentHealth -= amount;

	if (m_currentHealth <= 0 )
	{
		Kill();
	}
}

void TankHealth::Kill()
{
	m_currentHealth = 0;
	--m_currentLives;
	m_onDeath.Notify({m_currentLives});


	// TODO, handle this elsewhere using above event.
	ResetHealth();
}

void TankHealth::ResetHealth()
{
	m_currentHealth = m_health;
}

void TankHealth::ResetLives()
{
	m_currentLives = m_lives;
}

void TankHealth::AddLife(int count)
{
	m_currentLives += count;
}

void TankHealth::AddListener(dae::EventListener<PlayerLivesChangedEvent>* listener)
{
	m_onDeath.AddListener(listener);
	m_onDeath.Notify({ m_currentLives }); // Initial update
}

TankHealth::TankHealth(dae::GameObject& owner, int lives, int health)
	: Component(owner)
	, m_health{ health }
	, m_currentHealth{ health }
	, m_lives{ lives }
	, m_currentLives{ lives }
{
	if (lives <= 0)
	{
		throw std::out_of_range("Cannot create TankHealth with invalid Life count!");
	}
	if (health <= 0)
	{
		throw std::out_of_range("Cannot create TankHealth with invalid health!");
	}
}

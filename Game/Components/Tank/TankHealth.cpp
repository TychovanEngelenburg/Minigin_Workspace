#include "TankHealth.h"
#include "Game/Events/GameEvents.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/Events/IObserver.h>
#include "Game/Core/GameContext.h"

bool TankHealth::IsDead() const noexcept
{
	return m_Health <= 0;
}

int TankHealth::Health() const noexcept
{
	return m_Health;
}

void TankHealth::Damage(int amount, std::optional<int> killerId)
{
	m_Health -= amount;

	if (IsDead())
	{
		OnDeath(killerId);
	}

	// TODO: Hanlde another way?
	DemoPlaySound();
}

void TankHealth::Kill()
{
	m_Health = 0;
	OnDeath(std::nullopt);
}

void TankHealth::ResetHealth()
{
	m_Health = MaxHealth;
}

void TankHealth::SetPlayerId(std::optional<int> playerId)
{
	m_playerId = playerId;
}

void TankHealth::AddListener(mg::IObserver<TankDeathEvent>* listener)
{
	m_onDeath.AddListener(listener);
}

void TankHealth::SetScoreValue(int amount)
{
	m_scoreValue = amount;
}

void TankHealth::Start()
{
	m_Health = MaxHealth;
}

TankHealth::TankHealth(mg::GameObject& owner)
	: Component(owner)
{
}

void TankHealth::OnDeath(std::optional<int> killedBy)
{
	TankDeathEvent deathEvent{ m_playerId , killedBy, m_scoreValue };

	// TODO: Play sound?
	m_onDeath.Notify(deathEvent);
	Object()->SetActive(false);
}

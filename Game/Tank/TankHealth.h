#ifndef TANKHEALTH_H
#define TANKHEALTH_H
#include "EngineComponents/Component.h"
#include "EventSource.h"
#include "GameEvents.h"

namespace dae
{
	class GameObject;
};

class TankHealth final : public dae::Component
{
public:
	bool IsDead() const noexcept;
	bool CanRespawn() const noexcept;
	int GetHealth() const noexcept;
	int GetLives()const noexcept;

	void Damage(int amount = 1);
	void Kill();
	void ResetHealth();
	void ResetLives();
	void AddLife(int count = 1);
	void AddListener(dae::EventListener<PlayerLivesChangedEvent>* listener);

	TankHealth(dae::GameObject& owner, int lives = 4, int health = 1);

private:
	int const m_health;
	int m_currentHealth;

	int const m_lives;
	int m_currentLives;

	dae::EventSource<PlayerLivesChangedEvent> m_onDeath;
};
#endif // !TANKHEALTH_H

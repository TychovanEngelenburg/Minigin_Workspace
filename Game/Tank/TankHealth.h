#ifndef TANKHEALTH_H
#define TANKHEALTH_H
#include "EngineComponents/Component.h"
#include "EventSource.h"
#include "GameEvents.h"

namespace mg
{
	class GameObject;
};

class TankHealth final : public mg::Component
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
	void AddListener(mg::IEventListener<PlayerLivesChangedEvent>* listener);

	explicit TankHealth(mg::GameObject& owner, int lives = 4, int health = 1);

private:
	int const m_health;
	int m_currentHealth;

	int const m_lives;
	int m_currentLives;

	mg::EventSource<PlayerLivesChangedEvent> m_onDeath;
};
#endif // !TANKHEALTH_H

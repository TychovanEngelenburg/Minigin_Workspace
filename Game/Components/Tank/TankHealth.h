#ifndef TANKHEALTH_H
#define TANKHEALTH_H

#include "Game/Events/GameEvents.h"

#include <Minigin/Components/Component.h>
#include <Minigin/Events/EventSource.h>

#include <optional>

namespace mg
{
	class GameObject;
};

class TankHealth final : public mg::Component
{
public:
	bool IsDead() const noexcept;
	int Health() const noexcept;

	void Damage(int amount = 1, int killerId = -1);
	void Kill();
	void ResetHealth();
	void AddListener(mg::IEventListener<TankDeathEvent>* listener);
	void SetScoreValue(int amount);

	void Start() override;

	explicit TankHealth(mg::GameObject& owner);

	int MaxHealth{1};
	std::optional<int> OwnerPlayerId{std::nullopt };
private:
	int m_tankId{};
	int m_Health{};
	int m_scoreValue{};

	mg::EventSource<TankDeathEvent> m_onDeath;
	void OnDeath(std::optional<int> killedBy);

	// Temporary demonstration code
	//#include <Minigin/Audio/ISoundSystem.h>
	//#include <Minigin/Audio/SoundServiceLocator.h>
	void DemoPlaySound()
	{
		//mg::SoundServiceLocator::Fetch().PlaySFX({ "./Data/Audio_Tron1982/09_Sounds_MCP_Blip.wav", "damageTaken", 0 });
	}

};
#endif // !TANKHEALTH_H

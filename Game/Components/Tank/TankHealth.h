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

	void Damage(int amount = 1, std::optional<int> killerId = std::nullopt);
	void Kill();
	void ResetHealth();
	void SetPlayerId(std::optional<int> playerId);
	void AddListener(mg::IObserver<TankDeathEvent>* listener);
	void SetScoreValue(int amount);

	void Start() override;

	explicit TankHealth(mg::GameObject& owner);

	int MaxHealth{1};

private:
	std::optional<int> m_playerId{std::nullopt };
	int m_Health{};
	int m_scoreValue{};

	mg::Subject<TankDeathEvent> m_onDeath;
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

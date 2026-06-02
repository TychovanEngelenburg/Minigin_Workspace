#ifndef TANKHEALTH_H
#define TANKHEALTH_H
#include "Helper/GameEvents.h"

#include <Minigin/EngineComponents/Component.h>
#include <Minigin/EventSource.h>


namespace mg
{
	class GameObject;
};

class TankHealth final : public mg::Component
{
public:
	bool IsDead() const noexcept;
	int Health() const noexcept;


	void Damage(int amount = 1);
	void Kill();
	void ResetHealth();
	void AddListener(mg::IEventListener<PlayerDeath>* listener);

	explicit TankHealth(mg::GameObject& owner, int playerId);
	int maxHealth{1};

private:
	int m_playerId;
	int m_Health{1};


	mg::EventSource<PlayerDeath> m_onDeath;
	void OnDeath();

	// Temporary demonstration code
	//#include <Minigin/SoundSystem/ISoundSystem.h>
	//#include <Minigin/SoundSystem/SoundServiceLocator.h>
	void DemoPlaySound()
	{
		//mg::SoundServiceLocator::Fetch().PlaySFX({ "./Data/Audio_Tron1982/09_Sounds_MCP_Blip.wav", "damageTaken", 0 });
	}

};
#endif // !TANKHEALTH_H

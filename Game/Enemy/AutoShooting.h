#ifndef AI_SHOOTING_H
#define AI_SHOOTING_H
#include <Minigin/EngineComponents/Component.h>

class TankBarrel;
class GameGrid;
class TankManager;

class AutoShooting final : public mg::Component
{
public:
	void Awake() override;
	void Update() override;


	AutoShooting(mg::GameObject& owner, TankManager& tankManager, GameGrid& grid);

private:
	bool CheckLineOfSight();

	static float constexpr m_viewDistance{ 1000.f };
	static float constexpr m_viewCheckStep{ 4.f };

	TankBarrel* m_pBarrel{};
	TankManager* m_pTankmanager;
	GameGrid* m_pGrid;
};

#endif // !AI_SHOOTING_H
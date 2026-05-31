#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include <Minigin/EngineComponents/Component.h>
#include <vector>
#include <glm/vec2.hpp>

namespace mg
{
	class GameObject;
}

class BulletPool;
class GameGrid;
class TankManager final : public mg::Component
{
public:
	void SetBulletPool(BulletPool* pool);
	void Awake() override;
	TankManager(mg::GameObject& owner, GameGrid& grid);

private:
	//enum Team
	//{
	//	Good,
	//	Evil
	//};

	//enum TankType
	//{
	//	Player,
	//	BasicEnemy,
	//	Recogniser
	//};

	//struct PlayerInput
	//{
	//	int PlayerIndex;
	//	bool CanTurnBarrel;
	//};

	//struct ShootConfig
	//{
	//	bool AutoShoot{true};
	//	bool TurningBarrel{false};
	//	float ShootInterval{1.f};
	//};

	//struct TankConfig
	//{
	//	TankType Type{TankType::BasicEnemy};
	//	ShootConfig ShootConfig{};
	//	Team Team{Team::Evil};
	//	std::optional<PlayerInput> PlayerConfig;
	//};
	BulletPool* m_pBulletPool{};

	GameGrid* m_pGrid;
	std::vector<mg::GameObject*> m_pTanks{};

	mg::GameObject* SpawnPlayer(int playerId = 0, glm::ivec2 gridPos = { 0, 0 });
	void BindKeyboard(mg::GameObject* playerObj);
	void BindGamepad(mg::GameObject* playerObj, int playerId);
};


#endif // !PLAYER_MANAGER_H

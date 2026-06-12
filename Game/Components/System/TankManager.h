#ifndef TANK_MANAGER_H
#define TANK_MANAGER_H

#include "Game/Core/GameContext.h"
#include "Game/Core/PlayerDeviceMapper.h"

#include <Minigin/Components/Component.h>

#include <vector>
#include <glm/vec2.hpp>
#include <optional>

class BulletPool;
class GameGrid;
struct TankConfig;

class TankManager final : public mg::Component
{
public:
	struct SpawnCounts
	{
		int Players{ 0 };
		int Enemies{ 0 };
	};

	std::vector<glm::ivec2> OccupiedTiles() const;

	SpawnCounts SpawnTanks();
	void SetBulletPool(BulletPool* pool);

	TankManager(mg::GameObject& owner, GameGrid& grid);

private:
	void BindKeyboard(mg::GameObject& playerObj, mg::GameObject* barrelObj, bool canTurnBarrel);
	void BindGamepad(mg::GameObject& playerObj, mg::GameObject* barrelObj, bool canTurnBarrel, int playerId);

	mg::GameObject* SpawnTank(glm::ivec2 const& gridPos, TankConfig const& tankConfig, std::optional<PlayerSession> playerBinding = std::nullopt);

	GameGrid* m_pGrid;
	BulletPool* m_pBulletPool{ nullptr };
	std::vector<mg::GameObject*> m_pTanks{};

	mg::PlayerDeviceMapper m_deviceMapper{};
};


#endif // !TANK_MANAGER_H

#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "Game/Config/PlayerInputConfig.h"
#include "Game/Core/GameContext.h"
#include "Game/Core/PlayerDeviceMapper.h"

#include <Minigin/Components/Component.h>

#include <vector>
#include <glm/vec2.hpp>
#include <optional>

class BulletPool;
class GameGrid;
struct TankConfig;
struct PlayerInputConfig;

class TankManager final : public mg::Component
{
public:
	struct SpawnCounts
	{
		int Players{ 0 };
		int Enemies{ 0 };
	};

	mg::GameObject* SpawnTank(glm::ivec2 const& gridPos, TankConfig const& tankConfig, std::optional<PlayerInputConfig> inputConfig = std::nullopt);
	SpawnCounts Initialize(GameContext::GameMode const& mode);

	void SetBulletPool(BulletPool* pool);

	TankManager(mg::GameObject& owner, GameGrid& grid);

private:
	void BindKeyboard(mg::GameObject& playerObj, mg::GameObject* barrelObj, bool canTurnBarrel);
	void BindGamepad(mg::GameObject& playerObj, mg::GameObject* barrelObj, bool canTurnBarrel, int playerId);

	GameGrid* m_pGrid;
	BulletPool* m_pBulletPool{ nullptr };
	std::vector<mg::GameObject*> m_pTanks{};

	mg::PlayerDeviceMapper m_deviceMapper{};
};


#endif // !PLAYER_MANAGER_H

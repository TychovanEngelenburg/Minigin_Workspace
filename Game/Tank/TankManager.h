#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include <Minigin/EngineComponents/Component.h>
#include <vector>
#include <glm/vec2.hpp>
#include <optional>
#include "Game/Player/PlayerInputConfig.h"

namespace mg
{
	class GameObject;
}

class BulletPool;
class GameGrid;
struct TankConfig;

class TankManager final : public mg::Component
{
public:
	void SetBulletPool(BulletPool* pool);
	void Awake() override;
	TankManager(mg::GameObject& owner, GameGrid& grid);

private:

	BulletPool* m_pBulletPool{nullptr};

	GameGrid* m_pGrid;
	std::vector<mg::GameObject*> m_pTanks{};

	mg::GameObject* SpawnTank(glm::ivec2 const& gridPos, TankConfig const& tankConfig, std::optional<PlayerInputConfig> inputConfig = std::nullopt);

	// To be depricated!
	mg::GameObject* SpawnPlayer(int playerId = 0, glm::ivec2 gridPos = { 0, 0 });

	void BindKeyboard(mg::GameObject& playerObj, mg::GameObject* barrelObj, bool canTurnBarrel);
	void BindGamepad(mg::GameObject& playerObj, mg::GameObject* barrelObj, bool canTurnBarrel, int playerId);
};


#endif // !PLAYER_MANAGER_H

#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include <Minigin/EngineComponents/Component.h>
#include <vector>
#include <glm/vec2.hpp>

namespace mg
{
	class GameObject;
}

class GameGrid;
class PlayerManager final : public mg::Component
{
public:
	PlayerManager(mg::GameObject& owner, GameGrid& grid);


	void Awake() override;

private:
	GameGrid* m_pGrid;
	std::vector<mg::GameObject*> m_pPlayers{};

	mg::GameObject* SpawnPlayer(int playerId = 0, glm::ivec2 gridPos = { 0, 0 });
	void BindKeyboard(mg::GameObject* playerObj);
	void BindGamepad(mg::GameObject* playerObj, int playerId);
};


#endif // !PLAYER_MANAGER_H

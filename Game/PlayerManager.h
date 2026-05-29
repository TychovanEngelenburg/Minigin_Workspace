#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include <Minigin/EngineComponents/Component.h>
#include <vector>

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

	void SpawnPlayer(int playerId = 0);
	void BindKeyboard(mg::GameObject* playerObj);
	void BindGamepad(mg::GameObject* playerObj, int playerId);
};


#endif // !PLAYER_MANAGER_H

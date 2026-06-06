#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <Minigin/Components/Component.h>


class GameManager final : public mg::Component
{
public:
    void Update() override;

    GameManager(mg::GameObject& owner);
};

#endif // GAME_MANAGER_H
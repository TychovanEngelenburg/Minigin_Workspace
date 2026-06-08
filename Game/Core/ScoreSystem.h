#ifndef SCORE_SYSTEM_H
#define SCORE_SYSTEM_H

#include "Game/Events/GameEvents.h"

#include <Minigin/Events/IEventListener.h>

class ScoreSystem final : public mg::IEventListener<TankDeathEvent>
{
public:
    static void OnNotify(TankDeathEvent const& event) override;
};

#endif // !SCORE_SYSTEM_H


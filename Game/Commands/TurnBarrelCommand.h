#ifndef TURN_BARREL_COMMAND_H
#define TURN_BARREL_COMMAND_H

#include <Minigin/InputHandling/GameObjectCommand.h>

class TurnBarrelCommand final : public mg::GameObjectCommand
{
public:
	void Execute() override;
	TurnBarrelCommand(mg::GameObject& object, float degreesPerSec);
private:
	float m_degreesPerSec;
};

#endif // !TURN_BARREL_COMMAND_H

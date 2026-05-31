#ifndef SHOOT_COMMAND_H
#define SHOOT_COMMAND_H

#include <Minigin/InputHandling/GameObjectCommand.h>
class TankBarrel;

class ShootCommand final : public mg::GameObjectCommand
{
public:
	void Execute() override;
	ShootCommand(mg::GameObject& object);
private:
	TankBarrel* m_pBarrel;
};

#endif // !SHOOT_COMMAND_H

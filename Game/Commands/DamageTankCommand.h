#ifndef DAMAGETANKCOMMAND_H
#define DAMAGETANKCOMMAND_H
#include "glm/glm.hpp"
#include "InputHandling/GameObjectCommand.h" 

namespace mg
{
	class GameObject;
}

class TankHealth;

class DamageTankCommand final : public mg::GameObjectCommand
{
public:
	void Execute() override;

	explicit DamageTankCommand(mg::GameObject* object, int damageAmnt = 1);

	~DamageTankCommand() override = default;
	DamageTankCommand(DamageTankCommand const& other) = delete;
	DamageTankCommand(DamageTankCommand&& other) = delete;
	DamageTankCommand& operator=(DamageTankCommand const& other) = delete;
	DamageTankCommand& operator=(DamageTankCommand&& other) = delete;

private:
	TankHealth* m_pTankHealth;
	int m_damageAmount;
};
#endif // !DAMAGETANKCOMMAND_H
#ifndef DAMAGETANKCOMMAND_H
#define DAMAGETANKCOMMAND_H
#include "glm/glm.hpp"
#include "InputHandling/GameObjectCommand.h" 

namespace dae
{
	class GameObject;
}

class TankHealth;

class DamageTankCommand final : public dae::GameObjectCommand
{
public:
	void Execute() override;

	DamageTankCommand(dae::GameObject* object, int damageAmnt);

	~DamageTankCommand() override = default;
	DamageTankCommand(DamageTankCommand const& other) = delete;
	DamageTankCommand(DamageTankCommand&& other) = delete;
	DamageTankCommand& operator=(DamageTankCommand const& other) = delete;
	DamageTankCommand& operator=(DamageTankCommand&& other) = delete;
private:
	TankHealth* m_tankHealth;
	int m_damageAmount;
};
#endif // !DAMAGETANKCOMMAND_H
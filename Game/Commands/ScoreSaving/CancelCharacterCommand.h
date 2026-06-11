#ifndef CANCEL_CHARACTER_COMMAND_H
#define CANCEL_CHARACTER_COMMAND_H


#include <Minigin/Input/GameObjectCommand.h>

namespace mg
{
	class GameObject;
}

class ScoreSaver;
class CancelCharacter final : public mg::GameObjectCommand
{
public:
	void Execute() override;

	CancelCharacter(mg::GameObject& object);

private:
	ScoreSaver* m_pMovementComp;
};
#endif // !CANCEL_CHARACTER_COMMAND_H
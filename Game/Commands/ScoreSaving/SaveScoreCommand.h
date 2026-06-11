#ifndef SAVE_SCORE_COMMAND_H
#define SAVE_SCORE_COMMAND_H


#include <Minigin/Input/GameObjectCommand.h>

namespace mg
{
	class GameObject;
}

class ScoreSaver;
class SaveScoreCommand final : public mg::GameObjectCommand
{
public:
	void Execute() override;

	SaveScoreCommand(mg::GameObject& object);

private:
	ScoreSaver* m_pMovementComp;
};
#endif // !SAVE_SCORE_COMMAND_H
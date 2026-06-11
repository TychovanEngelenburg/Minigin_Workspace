#ifndef SCROLLLETTER_COMMAND_H
#define SCROLLLETTER_COMMAND_H


#include <Minigin/Input/GameObjectCommand.h>

namespace mg
{
	class GameObject;
}

class ScoreSaver;
class ScrollLetterCommand final : public mg::GameObjectCommand
{
public:
	void Execute() override;

	ScrollLetterCommand(mg::GameObject& object, bool scrollBack = false);

private:
	ScoreSaver* m_pMovementComp;
	bool m_scrollBack;
};
#endif // !SCROLLLETTER_COMMAND_H
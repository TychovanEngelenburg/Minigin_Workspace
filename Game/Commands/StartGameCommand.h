#ifndef START_GAME_COMMAND_H
#define START_GAME_COMMAND_H

#include <Minigin/InputHandling/ICommand.h>

namespace mg
{
	class GameObject;
}

class StartGameCommand final : public mg::ICommand
{
public:
	void Execute() override;
};
#endif // !START_GAME_COMMAND_H
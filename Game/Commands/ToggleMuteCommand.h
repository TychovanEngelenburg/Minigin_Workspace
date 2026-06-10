#ifndef TOGGLE_MUTE_COMMAND_H
#define TOGGLE_MUTE_COMMAND_H

#include <Minigin/Input/ICommand.h>

namespace mg
{
	class GameObject;
}

class ToggleMuteCommand final : public mg::ICommand
{
public:
	void Execute() override;
};
#endif // !TOGGLE_MUTE_COMMAND_H
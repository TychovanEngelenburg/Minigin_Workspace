#ifndef CONTINUE_SCENE_COMMAND_H
#define CONTINUE_SCENE_COMMAND_H

#include <Minigin/Input/ICommand.h>

namespace mg
{
	class GameObject;
}

class ContinueSceneCommand final : public mg::ICommand
{
public:
	void Execute() override;
};
#endif // !CONTINUE_SCENE_COMMAND_H
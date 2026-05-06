#ifndef START_GAME_COMMAND_H
#define START_GAME_COMMAND_H
#include "glm/glm.hpp"
#include "Minigin/InputHandling/GameObjectCommand.h" 

namespace mg
{
	class GameObject;
}

class StartGameCommand final : public mg::ICommand
{
public:
	void Execute() override;

	explicit StartGameCommand() = default;

	~StartGameCommand() override = default;
	StartGameCommand(StartGameCommand const& other) = delete;
	StartGameCommand(StartGameCommand&& other) = delete;
	StartGameCommand& operator=(StartGameCommand const& other) = delete;
	StartGameCommand& operator=(StartGameCommand&& other) = delete;
};
#endif // !START_GAME_COMMAND_H
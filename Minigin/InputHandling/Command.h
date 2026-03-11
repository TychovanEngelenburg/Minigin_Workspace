#ifndef COMMAND_H
#define COMMAND_H
#include "glm/glm.hpp"

namespace dae
{
	class Command
	{
	public:
		virtual void Execute() = 0;

		Command() = default;
		virtual ~Command() = default;
		Command(Command const& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(Command const& other) = delete;
		Command& operator=(Command&& other) = delete;
	};
}
#endif // !COMMAND_H

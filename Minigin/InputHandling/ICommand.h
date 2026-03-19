#ifndef ICOMMAND_H
#define ICOMMAND_H
namespace mg
{
	class ICommand
	{
	public:
		virtual void Execute() = 0;

		ICommand() = default;

		virtual ~ICommand() = default;
		ICommand(ICommand const& other) = delete;
		ICommand(ICommand&& other) = delete;
		ICommand& operator=(ICommand const& other) = delete;
		ICommand& operator=(ICommand&& other) = delete;
	};
}
#endif // !ICOMMAND_H

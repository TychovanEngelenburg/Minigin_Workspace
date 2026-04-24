#include "InputBinding.h"

int mg::InputBinding::DeviceIdx() const noexcept
{
	return m_deviceIndex;
}

int mg::InputBinding::InputCode() const noexcept
{
	return m_inputCode;
}

mg::ICommand* mg::InputBinding::Command() const noexcept
{
	return m_pCommand.get();
}

mg::InputBinding::DeviceType mg::InputBinding::GetType() const noexcept
{
	return m_deviceType;
}


mg::InputBinding::TriggerType mg::InputBinding::GetTrigger() const noexcept
{
	return m_triggersOn;
}

mg::InputBinding::InputBinding(int deviceIdx, int inputIdx, DeviceType deviceType, std::unique_ptr<ICommand> command,TriggerType trigger)
	: m_deviceIndex{ deviceIdx }
	, m_inputCode{ inputIdx }
	, m_deviceType{ deviceType }
	, m_pCommand{ std::move(command) }
	, m_triggersOn{ trigger }
{
}
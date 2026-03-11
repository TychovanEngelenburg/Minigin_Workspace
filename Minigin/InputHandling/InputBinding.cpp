#include "InputBinding.h"

int dae::InputBinding::GetDeviceIdx() const noexcept
{
	return m_deviceIndex;
}

int dae::InputBinding::GetInputCode() const noexcept
{
	return m_inputCode;
}

dae::Command* dae::InputBinding::GetCommand() const noexcept
{
	return m_command.get();
}

dae::InputBinding::DeviceType dae::InputBinding::GetType() const noexcept
{
	return m_deviceType;
}


dae::InputBinding::TriggerType dae::InputBinding::GetTrigger() const noexcept
{
	return m_triggersOn;
}

dae::InputBinding::InputBinding(int deviceIdx, int inputIdx, DeviceType deviceType, std::unique_ptr<Command> command,TriggerType trigger)
	: m_deviceIndex{ deviceIdx }
	, m_inputCode{ inputIdx }
	, m_deviceType{ deviceType }
	, m_command{ std::move(command) }
	, m_triggersOn{ trigger }
{
}
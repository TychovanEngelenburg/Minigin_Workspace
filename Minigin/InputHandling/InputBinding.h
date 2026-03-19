#ifndef INPUTBINDING_H
#define INPUTBINDING_H
#include <memory>
#include "InputHandling/ICommand.h"

namespace mg
{
	class InputBinding final
	{
    public:
        enum class DeviceType
        {
            Keyboard,
            Gamepad
            // TODO: Mouse support?
        };

        enum class TriggerType
        {
            Pressed,
            Released,
            Held,
            Axis
        };

        int GetDeviceIdx() const noexcept;
        int GetInputCode() const noexcept;

        ICommand* GetCommand() const noexcept;
        DeviceType GetType() const noexcept;
        TriggerType GetTrigger() const noexcept;
        
        InputBinding(int deviceIdx, int inputIdx, DeviceType deviceType, std::unique_ptr<ICommand> command,  TriggerType trigger = TriggerType::Pressed);

        ~InputBinding() = default;
        InputBinding(InputBinding const& other) = delete;
        InputBinding(InputBinding&& other) = delete;
        InputBinding& operator=(InputBinding const& other) = delete;
        InputBinding& operator=(InputBinding&& other) = delete;

    private:
        int m_deviceIndex;
        int m_inputCode;

        DeviceType m_deviceType;
        std::unique_ptr<ICommand> m_pCommand;
        TriggerType m_triggersOn;
	};

};

#endif // !INPUTBINDING_H

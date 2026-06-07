#ifndef INPUTBINDING_H
#define INPUTBINDING_H
#include <memory>
#include "Minigin/Input/ICommand.h"

namespace mg
{
	class InputBinding final
	{
    public:
        // TODO: Add BindingConfig (name TBD) struct? Do this in seperate .h file.
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

        int DeviceIdx() const noexcept;
        int InputCode() const noexcept;

        ICommand* Command() const noexcept;
        DeviceType GetType() const noexcept;
        TriggerType GetTrigger() const noexcept;
        

        // TODO: replace inputidx and devicetype with std::variant<Keycodes::KeyboardKey, Keycodes::GamepadButton, Keycodes::GamepadAxis> ;
        InputBinding(int deviceIdx, int inputIdx, DeviceType deviceType, std::unique_ptr<ICommand> command,  TriggerType trigger = TriggerType::Pressed);

    private:
        int m_deviceIndex;
        int m_inputCode;

        DeviceType m_deviceType;
        std::unique_ptr<ICommand> m_pCommand;
        TriggerType m_triggersOn;
	};

};

#endif // !INPUTBINDING_H

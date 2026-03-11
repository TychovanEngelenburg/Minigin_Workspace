#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H
#include <cstddef>

namespace dae
{
    class InputDevice
    {
    public:
        virtual ~InputDevice() = default;

        virtual bool GetButton(int button) const = 0;
        virtual bool GetButtonDown(int button) const = 0;
        virtual bool GetButtonUp(int button) const = 0;
    };
}
#endif // !INPUTDEVICE_H
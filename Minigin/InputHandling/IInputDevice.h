#ifndef IINPUTDEVICE_H
#define IINPUTDEVICE_H

namespace mg
{
    class IInputDevice
    {
    public:
        virtual bool GetButton(int button) const = 0;
        virtual bool GetButtonDown(int button) const = 0;
        virtual bool GetButtonUp(int button) const = 0;

        IInputDevice() = default;

        virtual ~IInputDevice() = default;
        IInputDevice(IInputDevice const& other) = delete;
        IInputDevice(IInputDevice&& other) = delete;
        IInputDevice& operator=(IInputDevice const& other) = delete;
        IInputDevice& operator=(IInputDevice&& other) = delete;
    };
}
#endif // !IINPUTDEVICE_H
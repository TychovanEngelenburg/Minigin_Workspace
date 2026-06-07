#ifndef PLAYER_DEVICE_MAPPER_H
#define PLAYER_DEVICE_MAPPER_H

#include <optional>
#include <cstddef>
#include <array>

namespace mg
{
    class InputSystem;

   
    class PlayerDeviceMapper final
    {
    public:
        bool PlayerUsesKeyboard(int playerId) const;
        std::optional<int> GamepadIndexForPlayer(int playerId) const;

        void Resolve(InputSystem const& input);

        PlayerDeviceMapper() = default;

    private:
        std::array<int, 2>  m_playerGamepadIds{ -1, -1 };
        std::array<bool, 2> m_playerUsesKeyboard{ true, false };
    };
}

#endif // PLAYER_DEVICE_MAPPER_H
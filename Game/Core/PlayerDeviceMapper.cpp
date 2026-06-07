#include "PlayerDeviceMapper.h"
#include <Minigin/Input/AbstractInputSystem.h>

std::optional<int> mg::PlayerDeviceMapper::GamepadIndexForPlayer(int playerId) const
{
    if (playerId < 0 || playerId >= m_playerGamepadIds.size())
    {
        return std::nullopt;
    }

    int slot = m_playerGamepadIds[playerId];
    if (slot < 0)
    {
        return std::nullopt;
    }

    return slot;
}

bool mg::PlayerDeviceMapper::PlayerUsesKeyboard(int playerId) const
{
    if (playerId < 0 || playerId >= 2)
    {
        return false;
    }

    return m_playerUsesKeyboard[playerId];
}

void mg::PlayerDeviceMapper::Resolve(InputSystem const& input)
{
    m_playerGamepadIds[0] = -1;
    m_playerGamepadIds[1] = -1;

    m_playerUsesKeyboard[0] = true;
    m_playerUsesKeyboard[1] = false;

    size_t const connected = input.ConnectedGamepadCount();

    if (connected == 0)
    {
        m_playerUsesKeyboard[0] = true;
        m_playerUsesKeyboard[1] = false;
    }
    else if (connected == 1)
    {
        m_playerGamepadIds[0] = -1; 
        m_playerGamepadIds[1] = 0;  
        m_playerUsesKeyboard[0] = true;
        m_playerUsesKeyboard[1] = false;
    }
    else
    {
        m_playerGamepadIds[0] = 0;
        m_playerGamepadIds[1] = 1;
        m_playerUsesKeyboard[0] = true;  
        m_playerUsesKeyboard[1] = false;
    }
}
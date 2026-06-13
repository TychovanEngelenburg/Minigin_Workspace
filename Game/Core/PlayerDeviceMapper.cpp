#include "PlayerDeviceMapper.h"

#include <Minigin/Input/AbstractInputSystem.h>

bool PlayerDeviceMapper::PlayerUsesKeyboard(size_t playerId) const noexcept
{
	return  m_playerWithKeyboard == playerId;
}

std::optional<size_t> PlayerDeviceMapper::GamepadIndexForPlayer(size_t playerId) const
{
	if (playerId < 0 || playerId >= m_playerGamepadIds.size())
	{
		return std::nullopt;
	}

	return m_playerGamepadIds[playerId];
}

void PlayerDeviceMapper::Resolve(mg::InputSystem const& input, size_t playerCount)
{
	if (playerCount <= 0)
	{
		return;
	}

	m_playerGamepadIds.clear();
	m_playerGamepadIds.resize(playerCount);

	m_disconnectedPads.clear();
	RemoveDisconnected(input);

	for (size_t i{ 0 }; i < playerCount; ++i)
	{
		auto const* pad = input.GetGamepad(i);
		if (pad && pad->Connected())
		{
			if (std::find(m_connectedPads.begin(), m_connectedPads.end(), i) == m_connectedPads.end())
			{
				m_connectedPads.push_back(i);
			}
		}
		else
		{
			m_disconnectedPads.push_back(i);
		}
	}

	AssignGamepads();
}

void PlayerDeviceMapper::RemoveDisconnected(mg::InputSystem const& input)
{
	std::erase_if(m_connectedPads, [&input](size_t slot)
	{
		auto const* pad = input.GetGamepad(slot);
		return !pad || !pad->Connected();
	});
}

void PlayerDeviceMapper::AssignGamepads()
{
	auto playerCount = m_playerGamepadIds.size();
	int gamepadId{ 0 };

	for (size_t i{ 0 }; i < playerCount; ++i)
	{
		if (i == 0)
		{
			if (playerCount > m_connectedPads.size())
			{
				m_playerGamepadIds[0] = m_disconnectedPads.back();
				continue;
			}
		}

		if (gamepadId < m_connectedPads.size())
		{
			m_playerGamepadIds[i] = m_connectedPads[gamepadId];
		}
		else
		{
			m_playerGamepadIds[i] = m_disconnectedPads[gamepadId - m_connectedPads.size()];
		}

		gamepadId++;
	}
}

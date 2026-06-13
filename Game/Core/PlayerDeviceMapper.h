#ifndef PLAYER_DEVICE_MAPPER_H
#define PLAYER_DEVICE_MAPPER_H

#include <optional>
#include <vector>

namespace mg
{
	class InputSystem;
}

class PlayerDeviceMapper final
{
public:
	bool PlayerUsesKeyboard(size_t playerId) const noexcept;
	std::optional<size_t> GamepadIndexForPlayer(size_t playerId) const;

	void Resolve(mg::InputSystem const& input, size_t playerCount);


private:
	void RemoveDisconnected(mg::InputSystem const& input);
	void AssignGamepads();

	static int constexpr m_playerWithKeyboard{ 0 };

	std::vector<size_t> m_playerGamepadIds{};
	std::vector<size_t> m_connectedPads{};

	std::vector<size_t> m_disconnectedPads;
};


#endif // PLAYER_DEVICE_MAPPER_H
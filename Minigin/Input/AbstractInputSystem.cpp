#include "AbstractInputSystem.h"

mg::IInputDevice const* mg::InputSystem::GetKeyboard() const
{
	return m_pKeyboard.get();
}

mg::IInputDevice const* mg::InputSystem::GetGamepad(size_t idx) const
{
	if (idx >= m_pGamepads.size())
	{
		return nullptr;
	}

	return m_pGamepads[idx].get();
}

size_t mg::InputSystem::ConnectedGamepadCount() const
{
	size_t count = 0;
	for (auto const& device : m_pGamepads)
	{
		if (device)
		{
			++count;
		}
	}
	return count;
}

void mg::InputSystem::Update()
{
	if (m_pKeyboard)
	{
		m_pKeyboard->Update();
	}

	for (auto& device : m_pGamepads)
	{
		if (device)
		{
			device->Update();
		}
	}
}

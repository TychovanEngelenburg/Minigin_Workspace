#include "Minigin/InputSystem/AbstractInputSystem.h"

mg::IInputDevice const* mg::InputSystem::GetKeyboard() const
{
	return m_pKeyboard.get();
}

mg::IInputDevice const* mg::InputSystem::GetGamepad(size_t idx) const
{
	return m_pGamepads[idx].get();
}

void mg::InputSystem::ProcessInput()
{
	m_pKeyboard->Update();
	for (auto& device : m_pGamepads)
	{
		if (device)
		{
			device->Update();
		}
	}
}

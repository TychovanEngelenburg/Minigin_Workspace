#include "InputManager.h"

mg::Keyboard const* mg::InputManager::GetKeyboard()
{
	return m_pKeyboard.get();
}

mg::Gamepad const* mg::InputManager::GetGamepad(size_t idx)
{
	return m_pGamepads[idx].get();
}

void mg::InputManager::ProcessInput()
{
	m_pKeyboard->Update();
	for (auto& device : m_pGamepads)
	{
		device->Update();
	}
}

void mg::InputManager::Init()
{
	m_pKeyboard = std::make_unique<Keyboard>();
	m_pGamepads[0] = std::make_unique<Gamepad>(0);
	m_pGamepads[1] = std::make_unique<Gamepad>(1);
	m_pGamepads[2] = std::make_unique<Gamepad>(2);
	m_pGamepads[3] = std::make_unique<Gamepad>(3);
}

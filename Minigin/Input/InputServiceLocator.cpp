#include "InputServiceLocator.h"

#include "Minigin/Platform/SDL_InputSystem.h"


std::unique_ptr<mg::InputSystem> mg::InputServiceLocator::m_pInstance = std::make_unique<mg::SDL_InputSystem>();

mg::InputSystem& mg::InputServiceLocator::Fetch()
{
	if (!m_pInstance)
	{
		Register(nullptr);
	}

	return *m_pInstance;
}

void mg::InputServiceLocator::Register(std::unique_ptr<mg::InputSystem> soundSystem)
{
	if (soundSystem == nullptr)
	{
		m_pInstance = std::make_unique<SDL_InputSystem>();
	}
	else
	{
		m_pInstance = std::move(soundSystem);
	}
}
#include "InputManager.h"

#include <SDL3/SDL_events.h>
#include <backends/imgui_impl_sdl3.h>


#include "InputBinding.h"
#include "InputHandling/ICommand.h"
#include "Gamepad.h"
#include "InputHandling/IInputDevice.h"

bool mg::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT) {
			return false;
		}
	}

	m_pKeyboard->Update();
	for (auto& device : m_pGamepads)
	{
		device->Update();
	}

	for (auto& binding : m_pBindings)
	{
		IInputDevice* device{};
		switch (binding->GetType())
		{
			case InputBinding::DeviceType::Gamepad:
			{
				device = m_pGamepads[binding->GetDeviceIdx()].get();
				break;
			}

			case InputBinding::DeviceType::Keyboard:
			default:
			{
				device = m_pKeyboard.get();
				break;
			}
		}

		

		switch (binding->GetTrigger())
		{
			case InputBinding::TriggerType::Pressed:
			{
				if (device->GetButtonDown(binding->GetInputCode()))
				{
					binding->GetCommand()->Execute();
				}
				break;
			}

			case InputBinding::TriggerType::Held:
			{
				if (device->GetButton(binding->GetInputCode()))
				{
					binding->GetCommand()->Execute();
				}
				break;
			}

			case InputBinding::TriggerType::Released:
			{
				if (device->GetButtonUp(binding->GetInputCode()))
				{
					binding->GetCommand()->Execute();
				}
				break;
			}

			case InputBinding::TriggerType::Axis:
			{
				//auto axis = device->GetAxis(binding->GetInputCode());
				//binding->GetCommand()->Execute(axis);
				break;
			}
		}
	}
	return true;
}

void mg::InputManager::AddBinding(std::unique_ptr<InputBinding> binding)
{
	m_pBindings.push_back(std::move(binding));
}

void mg::InputManager::RemoveBinding(InputBinding* binding)
{
	m_pBindings.erase(
		std::remove_if(
			m_pBindings.begin(),
			m_pBindings.end(),
			[binding](auto const& ptr) { return ptr.get() == binding; }
		),
		m_pBindings.end()
	);
}

void mg::InputManager::Init()
{
	m_pKeyboard = std::make_unique<Keyboard>();
	m_pGamepads[0] = std::make_unique<Gamepad>(0);
	m_pGamepads[1] = std::make_unique<Gamepad>(1);
	m_pGamepads[2] = std::make_unique<Gamepad>(2);
	m_pGamepads[3] = std::make_unique<Gamepad>(3);
}

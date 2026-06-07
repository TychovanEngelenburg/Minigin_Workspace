#include "SceneInput.h"

#include "Minigin/Input/InputBinding.h"
#include "Minigin/Input/IInputDevice.h"
#include "Minigin/Input/ICommand.h"
#include "Minigin/Input/InputServiceLocator.h"

void mg::SceneInput::ProcessInput()
{
	auto& inputService = InputServiceLocator::Fetch();

	for (auto& binding : m_pBindings)
	{
		IInputDevice const* device{ nullptr };

		switch (binding->GetType())
		{
			case InputBinding::DeviceType::Gamepad:
			{
				device = inputService.GetGamepad(binding->DeviceIdx());
				break;
			}

			case InputBinding::DeviceType::Keyboard:
			default:
			{
				device = inputService.GetKeyboard();
				break;
			}
		}

		if (!device)
		{
			continue;
		}

		switch (binding->GetTrigger())
		{
			case InputBinding::TriggerType::Pressed:
			{
				if (device->GetButtonDown(binding->InputCode()))
				{
					binding->Command()->Execute();
				}
				break;
			}

			case InputBinding::TriggerType::Held:
			{
				if (device->GetButton(binding->InputCode()))
				{
					binding->Command()->Execute();
				}
				break;
			}

			case InputBinding::TriggerType::Released:
			{
				if (device->GetButtonUp(binding->InputCode()))
				{
					binding->Command()->Execute();
				}
				break;
			}

			case InputBinding::TriggerType::Axis:
			{
				//auto axis = device->GetAxis(binding->InputCode());
				//binding->Command()->HandleEvent(axis);
				break;
			}
		}
	}
	return;
}

void mg::SceneInput::AddBinding(std::unique_ptr<InputBinding> binding)
{
	m_pBindings.push_back(std::move(binding));
}

void mg::SceneInput::RemoveBinding(InputBinding* binding)
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

void mg::SceneInput::ClearBindings()
{
	m_pBindings.clear();
}

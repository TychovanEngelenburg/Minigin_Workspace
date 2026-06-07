#ifndef INPUT_SERVICE_LOCATOR_H
#define INPUT_SERVICE_LOCATOR_H
#include "Minigin/Input/AbstractInputSystem.h"
#include <memory>

namespace mg
{
	class InputServiceLocator final
	{
	public:
		static InputSystem& Fetch();
		static void Register(std::unique_ptr<InputSystem> soundSystem);

		// No instances allowed, use class only statically
		InputServiceLocator() = delete;

	private:
		static std::unique_ptr<mg::InputSystem> m_pInstance;
	};
}

#endif // !INPUT_SERVICE_LOCATOR_H
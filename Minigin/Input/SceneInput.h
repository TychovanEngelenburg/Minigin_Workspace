#ifndef SCENE_INPUT_H
#define SCENE_INPUT_H
#include "Minigin/Input/InputBinding.h"

#include <vector>
#include <memory>
namespace mg
{
	class SceneInput final
	{
	public:
		void ProcessInput();

		void AddBinding(std::unique_ptr<InputBinding> binding);
		void RemoveBinding(InputBinding* binding);

		void ClearBindings();

	private:
		std::vector<std::unique_ptr<InputBinding>> m_pBindings{};
	};
};
#endif // !SCENE_INPUT_H

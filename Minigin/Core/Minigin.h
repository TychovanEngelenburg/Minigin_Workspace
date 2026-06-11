#ifndef MINIGIN_H
#define MINIGIN_H

#include "Minigin/Core/DeltaClock.h"

#include <filesystem>
#include <functional>
#include <memory>

namespace mg
{
	struct EngineConfig;
	class Minigin final
	{
	public:
		void Run(const std::function<void()>& load);
		void RunOneFrame();

		explicit Minigin(mg::EngineConfig const& windowConfig);

		~Minigin();
		Minigin(Minigin const& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(Minigin const& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

	private:
		std::unique_ptr<DeltaClock> m_pDeltaClock;
		bool m_quit{ false };
	};
}
#endif // !MINIGIN_H

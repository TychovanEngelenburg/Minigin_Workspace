#ifndef MINIGIN_H
#define MINIGIN_H

#include "DeltaClock.h"

#include <filesystem>
#include <functional>
#include <memory>

namespace mg
{
	class Minigin final
	{
	public:
		void Run(const std::function<void()>& load);
		void RunOneFrame();

		explicit Minigin(std::filesystem::path const& dataPath);
		~Minigin();
		Minigin(Minigin const& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(Minigin const& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	private:
		std::unique_ptr<DeltaClock> m_pDeltaClock;
		double m_lag;
		bool m_quit;
	};
}
#endif // !MINIGIN_H

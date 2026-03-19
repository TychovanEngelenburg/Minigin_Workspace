#ifndef DELTACLOCK_H
#define DELTACLOCK_H

#include <chrono>

namespace mg
{
	class DeltaClock final
	{
	public:
		static double GetDeltaTime() noexcept;
		static constexpr double GetFixedDeltaTime() noexcept 
		{
			static_assert(m_fixedDeltaTime > 0.0, "Deltatime must be a non-zero, positive value!");
			return m_fixedDeltaTime;
		};

		void Update();

		DeltaClock();

	private:
		static double constexpr m_fixedDeltaTime{ 0.02 };
		static double m_deltaTime;
		std::chrono::high_resolution_clock::time_point m_lastTime;
	};
}
#endif // !DELTACLOCK_H

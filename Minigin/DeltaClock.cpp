#include "DeltaClock.h"

// .h includes
#include <chrono>

double mg::DeltaClock::m_deltaTime = 0.0;

double mg::DeltaClock::GetDeltaTime() noexcept
{
	return m_deltaTime;
}

void mg::DeltaClock::Update()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	m_deltaTime = std::chrono::duration<double>( currentTime - m_lastTime).count();
	m_lastTime = currentTime;
}

mg::DeltaClock::DeltaClock()
	: m_lastTime{ std::chrono::high_resolution_clock::now() }
{
}

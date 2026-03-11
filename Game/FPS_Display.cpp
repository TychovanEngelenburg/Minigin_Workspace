#include "FPS_Display.h"

#include "DeltaClock.h"
#include "EngineComponents/Component.h"
#include "EngineComponents/TextComponent.h"
#include "GameObject.h"

#include <cassert>
#include <format>
#include <string>
#include <iomanip>
#include <ios>
#include <sstream>

#pragma region Game_Loop
void dae::FPS_Display::Start()
{
	m_textComp = GetOwner()->GetComponent<TextComponent>();
	assert(m_textComp && "FPS_Display requires a TextComponent on it's GameObject!");
}

void dae::FPS_Display::Update()
{
	static double constexpr m_accuTimePerSec{ 1 };

	m_accuTime += DeltaClock::GetDeltaTime();
	++m_frameCount;
	if (m_accuTime >= m_accuTimePerSec)
	{
		m_averageFPS = m_frameCount / m_accuTime;

		UpdateDisplay();
		m_accuTime -= m_accuTimePerSec;
		m_frameCount -= m_averageFPS;
	}
}
#pragma endregion Game_Loop

void dae::FPS_Display::UpdateDisplay()
{
#if __EMSCRIPTEN__
	std::ostringstream stream{};
	stream << std::fixed << std::setprecision(1) << m_averageFPS << " FPS";
	std::string const fpsText = stream.str();
#else
	std::string const fpsText{ std::format("{:.1f} FPS", m_averageFPS) };
#endif
	m_textComp->SetText(fpsText);
}

dae::FPS_Display::FPS_Display(GameObject& owner)
	: Component(owner)
	, m_textComp{}
	, m_accuTime{}
	, m_frameCount{}
	, m_averageFPS{}
{
}

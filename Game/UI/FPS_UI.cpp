#include "UI/FPS_UI.h"

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
void FPS_UI::Start()
{
	m_textComp = GetOwner()->GetComponent<dae::TextComponent>();
	assert(m_textComp && "FPS_UI requires a TextComponent on it's GameObject!");
}

void FPS_UI::Update()
{
	static double constexpr m_accuTimePerSec{ 1 };

	m_accuTime += dae::DeltaClock::GetDeltaTime();
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

void FPS_UI::UpdateDisplay()
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

FPS_UI::FPS_UI(dae::GameObject& owner)
	: dae::Component(owner)
	, m_textComp{}
	, m_accuTime{}
	, m_frameCount{}
	, m_averageFPS{}
{
}

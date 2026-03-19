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
	m_pTextComp = GetOwner()->GetComponent<mg::TextComponent>();
	assert(m_pTextComp && "FPS_UI requires a TextComponent on it's GameObject!");
}

void FPS_UI::Update()
{
	static double constexpr m_accuTimePerSec{ 1 };

	m_accuTime += mg::DeltaClock::GetDeltaTime();
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
	m_pTextComp->SetText(fpsText);
}

FPS_UI::FPS_UI(mg::GameObject& owner)
	: mg::Component(owner)
	, m_pTextComp{}
	, m_accuTime{}
	, m_frameCount{}
	, m_averageFPS{}
{
}

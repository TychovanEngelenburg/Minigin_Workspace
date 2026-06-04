#include "FPS_UI.h"

#include <Minigin/Core/DeltaClock.h>
#include <Minigin/Components/Component.h>
#include <Minigin/Components/TextComponent.h>
#include <Minigin/Scene/GameObject.h>

#include <cassert>
#include <format>
#include <string>

#pragma region Game_Loop
void FPS_UI::Awake()
{
	m_pTextComp = Owner()->GetComponent<mg::TextComponent>();
	assert(m_pTextComp && "FPS_UI requires a TextComponent on it's GameObject!");
}

void FPS_UI::Update()
{
	static double constexpr m_accuTimePerSec{ 1 };

	m_accuTime += mg::DeltaClock::DeltaTime();
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
{
}

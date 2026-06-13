#include "ScoreSaver.h"

#include "Game/Core/ScoreWriter.h"
#include "Game/Core/GameContext.h"

#include <Minigin/Components/TextComponent.h>
#include <Minigin/Scene/GameObject.h>

#include <cassert>

void ScoreSaver::CycleLetterUp()
{
	++m_selectedCharacter;
	if (m_selectedCharacter > LetterRange::Last)
	{
		m_selectedCharacter = LetterRange::First;
	}

	m_currentLetters[m_currentLetters.size() - 1] = m_selectedCharacter;
	m_pTextComp->SetText(GetName(m_currentLetters));
}

void ScoreSaver::CycleLetterDown()
{
	--m_selectedCharacter;
	if (m_selectedCharacter < LetterRange::First)
	{
		m_selectedCharacter = LetterRange::Last;
	}

	m_currentLetters[m_currentLetters.size() - 1] = m_selectedCharacter;
	m_pTextComp->SetText(GetName(m_currentLetters));

}

void ScoreSaver::Continue()
{

	m_currentLetters.push_back(m_selectedCharacter);
	m_pTextComp->SetText(GetName(m_currentLetters));

	if (m_currentLetters.size() > PlayerScore::NameLength)
	{
		auto& context = GameContext::Instance();

		std::string name{};
		for (size_t i = 0; i < PlayerScore::NameLength; i++)
		{
			name += m_currentLetters[i];
		
		}
		context.ScoreManager().AddScore(context.Mode(), {context.TotalScore(), name});
		context.ScoreManager().Save();

		context.PushEvent(GameEvent::Continue);
	}
}

void ScoreSaver::Back()
{
	if (m_currentLetters.size() > 1)
	{
		m_currentLetters.pop_back();
		m_pTextComp->SetText(GetName(m_currentLetters));
	}
}

void ScoreSaver::Awake()
{
	m_pTextComp = Object()->GetComponent<mg::TextComponent>();
	assert(m_pTextComp && "Bullet gameobject must have a collider!");

	m_pTextComp->SetText(GetName(m_currentLetters));
}

ScoreSaver::ScoreSaver(mg::GameObject& owner)
	: Component(owner)
{
}

std::string ScoreSaver::GetName(std::vector<char> const& chars)
{
	std::string name{};
	for (size_t i = 0; i < PlayerScore::NameLength; i++)
	{
		if (i >= chars.size())
		{
			name += "-";
			continue;
		}

		name += chars[i];
	}

	return name;
}

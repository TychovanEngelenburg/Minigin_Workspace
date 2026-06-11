#ifndef  SCORE_SAVER_H
#define SCORE_SAVER_H

#include <Minigin/Components/Component.h>

#include <vector>
#include <string>

namespace mg
{
	class TextComponent;
}

class ScoreSaver final : public mg::Component
{
public:
	void CycleLetterUp();
	void CycleLetterDown();


	void Continue();
	void Back();

	void Awake() override;

	ScoreSaver(mg::GameObject& owner);

private:

	std::string GetName(std::vector<char> const& chars);

	struct LetterRange
	{
		static char constexpr First{ 'A' };
		static char constexpr Last{ 'Z' };
	};

	char m_selectedCharacter{LetterRange::First};

	std::vector<char> m_currentLetters{ LetterRange::First };
	mg::TextComponent* m_pTextComp{};
};

#endif // ! SCORE_SAVER_H

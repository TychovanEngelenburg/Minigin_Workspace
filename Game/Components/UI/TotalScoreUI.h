#ifndef TOTAL_SCORE_UI_H
#define TOTAL_SCORE_UI_H

#include <Minigin/Components/Component.h>

namespace mg
{
	class TextComponent;
}

class TotalScoreUI final : public mg::Component
{
public:
	void UpdateText();

	void Awake() override;

	TotalScoreUI(mg::GameObject& owner);

private:
	mg::TextComponent* m_pTextComp{};
};

#endif // !TOTAL_SCORE_UI_H

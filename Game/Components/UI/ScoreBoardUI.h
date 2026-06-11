#ifndef SCOREBOARD_UI
#define SCOREBOARD_UI

#include <Minigin/Components/Component.h>

namespace mg
{
    class TextComponent;
}

class ScoreBoardUI : public mg::Component
{
public:
    void UpdateText();

    void Awake() override;

    ScoreBoardUI(mg::GameObject& owner);

private:
    mg::TextComponent* m_pTextComp{};
};

#endif // !SCOREBOARD_UI
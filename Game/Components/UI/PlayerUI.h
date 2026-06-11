#ifndef PLAYER_UI_H
#define PLAYER_UI_H

#include "Game/Events/UIEvents.h"

#include <Minigin/Components/Component.h>
#include <Minigin/Events/IObserver.h>

#include <string_view>

namespace mg
{
	class TextComponent;
}

class PlayerUI final : public mg::Component, public mg::IObserver<ScoreChangedEvent>, public mg::IObserver<LivesChangedEvent>
{
public:
	void SetPlayerId(int id);
	void SetScoreComp(mg::TextComponent* pTextComp);
	void SetSLivesComp(mg::TextComponent* pTextComp);

	void OnNotify(ScoreChangedEvent const& eventData) override;
	void OnNotify(LivesChangedEvent const& eventData) override;

	PlayerUI(mg::GameObject& owner);

private:
	static std::string_view constexpr m_noLivesText{ "OUT!" };

	mg::TextComponent* m_pScoreText{};
	mg::TextComponent* m_pLivesText{};

	int m_playerId{ 0 };
};

#endif // !PLAYER_UI_H

#ifndef GAMEMODE_UI
#define GAMEMODE_UI

#include <Minigin/Components/Component.h>
#include <Minigin/Events/IObserver.h>
#include "Game/Events/UIEvents.h"

namespace mg
{
	class TextComponent;
}

class GameModeUI final : public mg::Component, public mg::IObserver<GameModeChangedEvent>
{
public:
	void Awake() override;
	void Start() override;

	void OnNotify(GameModeChangedEvent const& event) override;

	GameModeUI(mg::GameObject& owner);
	~GameModeUI();

private:
	void UpdateText(GameMode mode);
	mg::TextComponent* m_pTextComp{};
};
#endif // !GAMEMODE_UI
#ifndef FPS_UI_H
#define FPS_UI_H

#include <Minigin/Components/Component.h>

namespace mg
{
	class TextComponent;
	class GameObject;
}

class FPS_UI final : public mg::Component
{
public:
	void Awake() override;
	void Update() override;

	explicit FPS_UI(mg::GameObject& owner);

private:
	mg::TextComponent* m_pTextComp{};
	double m_accuTime{};
	double m_frameCount{};
	double m_averageFPS{};

	void UpdateDisplay();
};
#endif // !FPS_UI_H

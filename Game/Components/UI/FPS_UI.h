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

	virtual ~FPS_UI() = default;
	FPS_UI(FPS_UI const& other) = delete;
	FPS_UI(FPS_UI&& other) = delete;
	FPS_UI& operator=(FPS_UI const& other) = delete;
	FPS_UI& operator=(FPS_UI&& other) = delete;

private:
	mg::TextComponent* m_pTextComp;
	double m_accuTime;
	double m_frameCount;
	double m_averageFPS;

	void UpdateDisplay();
};
#endif // !FPS_UI_H

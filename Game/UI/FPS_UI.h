#ifndef FPS_UI_H
#define FPS_UI_H

#include "EngineComponents/Component.h"

namespace dae
{
	class TextComponent;
	class GameObject;
}

class FPS_UI final : public dae::Component
{
public:
	void Start() override;
	void Update() override;

	FPS_UI(dae::GameObject& owner);

	~FPS_UI() override = default;
	FPS_UI(FPS_UI const& other) = delete;
	FPS_UI(FPS_UI&& other) = delete;
	FPS_UI& operator=(FPS_UI const& other) = delete;
	FPS_UI& operator=(FPS_UI&& other) = delete;

private:
	dae::TextComponent* m_textComp;
	double m_accuTime;
	double m_frameCount;
	double m_averageFPS;

	void UpdateDisplay();
};
#endif // !FPS_UI_H

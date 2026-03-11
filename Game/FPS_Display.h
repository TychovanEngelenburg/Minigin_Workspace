#ifndef FPS_DISPLAY_H
#define FPS_DISPLAY_H

#include "EngineComponents/Component.h"

namespace dae
{
	class TextComponent;
	class GameObject;
	class FPS_Display final : public Component
	{
	public:
		void Start() override;
		void Update() override;

		FPS_Display(GameObject& owner);

		~FPS_Display() override = default;
		FPS_Display(FPS_Display const& other) = delete;
		FPS_Display(FPS_Display&& other) = delete;
		FPS_Display& operator=(FPS_Display const& other) = delete;
		FPS_Display& operator=(FPS_Display&& other) = delete;

	private:
		TextComponent* m_textComp;
		double m_accuTime;
		double m_frameCount;
		double m_averageFPS;

		void UpdateDisplay();
	};
}
#endif // !FPS_DISPLAY_H

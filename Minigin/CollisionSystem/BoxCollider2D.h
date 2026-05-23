#ifndef COLLIDER_2D_H
#define COLLIDER_2D_H

#include <glm/vec2.hpp>
#include "Minigin/EngineComponents/Component.h"

namespace mg
{

	struct OBB
	{
		glm::vec2 center{};
		glm::vec2 axisX{};
		glm::vec2 axisY{};
		glm::vec2 halfExtends{};

	};

	class BoxCollider2D final : public Component
	{
	public:
		struct Bounds
		{
			glm::vec2 offset{};
			glm::vec2 size{};
		};


		void SetCenter(glm::vec2 center);
		void SetTopLeft(glm::vec2 topLeft);
		void SetSize(glm::vec2 size, bool scaleAroundCenter);

		Bounds const& LocalBounds() const;
		mg::OBB GetOBB() const;
		BoxCollider2D(mg::GameObject& owner, glm::vec2 size = {}, glm::vec2 offset = {});

	private:
		Bounds m_bounds;
	};
};

#endif // !COLLIDER_2D_H

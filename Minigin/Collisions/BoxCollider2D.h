#ifndef COLLIDER_2D_H
#define COLLIDER_2D_H

#include "Minigin/Components/Component.h"

#include <glm/vec2.hpp>
#include <cstdint>

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
		void SetSize(glm::vec2 size, bool scaleAroundCenter = false);

		Bounds const& LocalBounds() const;
		mg::OBB GetOBB() const;

		void OnEnable() override;
		void OnDisable() override;

		void Render() const override;

		explicit BoxCollider2D(mg::GameObject& owner);
		uint32_t CollisionLayer { 0 };
		uint32_t CollisionMask { 0xFFFFFFFF };

	private:
		Bounds m_bounds{};
	};
};

#endif // !COLLIDER_2D_H

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
		void SetSize(glm::vec2 size, bool scaleAroundCenter = false);

		Bounds const& LocalBounds() const;
		mg::OBB GetOBB() const;

		void Awake() override;
		void Render() const override;
		explicit BoxCollider2D(mg::GameObject& owner, glm::vec2 size = {1.f, 1.f}, glm::vec2 offset = {});

		~BoxCollider2D();
		BoxCollider2D(BoxCollider2D const& other) = delete;
		BoxCollider2D(BoxCollider2D&& other) = delete;
		BoxCollider2D& operator=(BoxCollider2D const& other) = delete;
		BoxCollider2D& operator=(BoxCollider2D&& other) = delete;

	private:
		Bounds m_bounds;
	};
};

#endif // !COLLIDER_2D_H

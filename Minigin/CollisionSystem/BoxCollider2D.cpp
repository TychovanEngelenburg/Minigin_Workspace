#include "Minigin/CollisionSystem/BoxCollider2D.h"
#include "Minigin/Scene/GameObject.h"

void mg::BoxCollider2D::SetCenter(glm::vec2 center)
{
	SetTopLeft({ center.x - m_bounds.size.x / 2.f, center.y - m_bounds.size.y / 2.f });
}

void mg::BoxCollider2D::SetTopLeft(glm::vec2 topLeft)
{
	m_bounds.offset.x = topLeft.x;
	m_bounds.offset.y = topLeft.y;
}

void mg::BoxCollider2D::SetSize(glm::vec2 size, bool scaleAroundCenter)
{
	if (scaleAroundCenter)
	{
		glm::vec2 center{
			m_bounds.offset.x + m_bounds.size.x / 2,
			m_bounds.offset.y + m_bounds.size.y / 2
		};

		SetTopLeft({ center.x - size.x / 2.f, center.y - size.y / 2.f });
	}

	m_bounds.size = size;
}

mg::BoxCollider2D::Bounds const& mg::BoxCollider2D::LocalBounds() const
{
	return m_bounds;
}

mg::OBB mg::BoxCollider2D::GetOBB() const
{
	OBB newOBB{};
	auto const& worldPos{ Owner()->Transform().WorldPosition() };

	newOBB.center.x = worldPos.x + m_bounds.offset.x;
	newOBB.center.y = worldPos.y + m_bounds.offset.y;


	auto const& worldRot{ Owner()->Transform().WorldRotationZ() };

	newOBB.axisX = glm::normalize(glm::vec2(cos(worldRot), sin(worldRot)));
	newOBB.axisY = glm::normalize(glm::vec2(-sin(worldRot), cos(worldRot)));

	newOBB.halfExtends.x = m_bounds.size.x / 2.f;
	newOBB.halfExtends.y = m_bounds.size.y / 2.f;

	return newOBB;
}

mg::BoxCollider2D::BoxCollider2D(mg::GameObject& owner, glm::vec2 size, glm::vec2 offset)
	: Component(owner)
	, m_bounds{ offset, size }
{
}

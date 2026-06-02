#include "Minigin/CollisionSystem/BoxCollider2D.h"
#include "Minigin/Scene/GameObject.h"
#include "Minigin/Scene/Scene.h"

#include <Minigin/Renderer.h>

void mg::BoxCollider2D::SetCenter(glm::vec2 center)
{
	m_bounds.offset = center - m_bounds.size * 0.5f;
}

void mg::BoxCollider2D::SetTopLeft(glm::vec2 topLeft)
{
	m_bounds.offset = topLeft;
}

void mg::BoxCollider2D::SetSize(glm::vec2 size, bool scaleAroundCenter)
{
	if (scaleAroundCenter)
	{
		glm::vec2 center{ m_bounds.offset + m_bounds.size * 0.5f };
		glm::vec2 newTopLeft = center - size * 0.5f;

		m_bounds.offset = newTopLeft;
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
	auto const& worldRot{ Owner()->Transform().WorldRotationZ() };

	glm::vec2 centerPos{ m_bounds.offset + m_bounds.size * .5f };

	glm::vec2 rotatedOffset = glm::vec2(
		cos(worldRot) * centerPos.x - sin(worldRot) * centerPos.y,
		sin(worldRot) * centerPos.x + cos(worldRot) * centerPos.y
	);

	newOBB.center = worldPos + rotatedOffset;


	newOBB.axisX = glm::normalize(glm::vec2(cos(worldRot), sin(worldRot)));
	newOBB.axisY = glm::normalize(glm::vec2(-sin(worldRot), cos(worldRot)));

	newOBB.halfExtends = m_bounds.size * .5f;

	return newOBB;
}

void mg::BoxCollider2D::Awake()
{
	Owner()->Scene()->CollisionSystem().Register(this);
}

void mg::BoxCollider2D::Render() const
{


	const auto& obb{ GetOBB() };

	Renderer::Instance().DrawRect({
		obb.center.x - m_bounds.size.x * 0.5f,
		obb.center.y - m_bounds.size.y * 0.5f,
		obb.halfExtends.x * 2.f,
		obb.halfExtends.y * 2.f
		});
}

mg::BoxCollider2D::BoxCollider2D(mg::GameObject& owner, glm::vec2 size, glm::vec2 offset)
	: Component(owner)
	, m_bounds{ offset, size }
{
}

mg::BoxCollider2D::~BoxCollider2D()
{
	Owner()->Scene()->CollisionSystem().Unregister(this);
}

#include "Minigin/CollisionSystem/CollisionSystem.h"
#include "Minigin/CollisionSystem/BoxCollider2D.h"
#include "Minigin/Scene/GameObject.h"

#include <glm/geometric.hpp>
#include <cassert>
#include <algorithm>


 mg::RaycastHit mg::SceneCollisions::Raycast(glm::vec2 const& origin, glm::vec2 const& direction, float maxDistance, uint32_t layerMask)
{
	mg::RaycastHit result;

	glm::vec2 normalizedDir = glm::normalize(direction);
	glm::vec2 rayEnd = origin + normalizedDir * maxDistance;

	float distance = maxDistance;

	for (auto* collider : m_pColliders)
	{
		if (!collider ||
			collider->CollisionMask == 0 ||
			!LineIntersectsOBB(origin, rayEnd, collider->GetOBB()) ||
			!(layerMask & collider->CollisionLayer))
		{
			continue;
		}


		glm::vec2 center = collider->GetOBB().center;
		float hitDist = glm::length(center - origin);

		if (hitDist < distance)
		{
			distance = hitDist;

			result.Hit = true;
			result.Collider = collider;
			result.Distance = hitDist;
			result.Point = origin + normalizedDir * hitDist;
		}
	}

	return result;
}

void mg::SceneCollisions::Register(BoxCollider2D* pCollider)
{
	assert(pCollider != nullptr);

	m_pColliders.push_back(pCollider);
}

void mg::SceneCollisions::Unregister(BoxCollider2D* pCollider)
{
	std::erase(m_pColliders, pCollider);
}

void mg::SceneCollisions::Update()
{
	std::vector<std::pair<BoxCollider2D*, BoxCollider2D*>> collidingPairs{};
	collidingPairs.reserve(collidingPairs.size());

	for (size_t indexA = 0; indexA < m_pColliders.size(); indexA++)
	{
		for (size_t indexB = indexA + 1; indexB < m_pColliders.size(); indexB++)
		{
			auto colliderA{ m_pColliders[indexA] };
			auto colliderB{ m_pColliders[indexB] };
			if (!CanCollide(*colliderA, *colliderB))
			{
				continue;
			}


			if (CheckOverlap(*colliderA, *colliderB))
			{
				auto pair{
					colliderA < colliderB ?
						std::make_pair(colliderA, colliderB) :
						std::make_pair(colliderB, colliderA)
				};

				if (std::find(m_pCollidingPairs.begin(), m_pCollidingPairs.end(), pair) == m_pCollidingPairs.end())
				{
					pair.first->Owner()->OnCollisionEnter({ pair.second });
					pair.second->Owner()->OnCollisionEnter({ pair.first });
				}
				else
				{
					pair.first->Owner()->OnCollisionStay({ pair.second });
					pair.second->Owner()->OnCollisionStay({ pair.first });
				}

				collidingPairs.push_back(pair);
			}
		}
	}

	for (auto const& pair : m_pCollidingPairs)
	{
		if (std::find(collidingPairs.begin(), collidingPairs.end(), pair) == collidingPairs.end())
		{
			pair.first->Owner()->OnCollisionExit({ pair.second });
			pair.second->Owner()->OnCollisionExit({ pair.first });
		}
	}

	m_pCollidingPairs = collidingPairs;
}

bool OverlapOnAxis(mg::OBB const& a, mg::OBB const& b, glm::vec2 const& axis)
{
	glm::vec2 distVec = b.center - a.center;
	float distance = std::abs(glm::dot(distVec, axis));

	float aProjection{
		a.halfExtends.x * std::abs(glm::dot(a.axisX, axis)) +
		a.halfExtends.y * std::abs(glm::dot(a.axisY, axis))
	};
	float bProjection{
		b.halfExtends.x * std::abs(glm::dot(b.axisX, axis)) +
		b.halfExtends.y * std::abs(glm::dot(b.axisY, axis))
	};

	return distance <= (aProjection + bProjection);
}

bool mg::SceneCollisions::CanCollide(BoxCollider2D const& a, BoxCollider2D const& b)
{
	return (a.CollisionMask & b.CollisionLayer) && (b.CollisionMask & a.CollisionLayer);
}


bool mg::SceneCollisions::CheckOverlap(BoxCollider2D const& A, BoxCollider2D const& B)
{
	OBB obbA{ A.GetOBB() };
	OBB obbB{ B.GetOBB() };


	if (!OverlapOnAxis(obbA, obbB, obbA.axisX))
	{
		return false;
	}

	if (!OverlapOnAxis(obbA, obbB, obbA.axisY))
	{
		return false;
	}

	if (!OverlapOnAxis(obbA, obbB, obbB.axisX))
	{
		return false;
	}

	if (!OverlapOnAxis(obbA, obbB, obbB.axisY))
	{
		return false;
	}

	return true;
}

bool mg::SceneCollisions::OverLap(float minA, float maxA, float minB, float maxB)
{
	return (minB <=  maxA && minA <=  maxB);
}

void  mg::SceneCollisions::ProjectSegment(glm::vec2 a, glm::vec2 b, glm::vec2 const& axis, float& min, float& max)
{
	float pointA = glm::dot(a, axis);
	float pointB = glm::dot(b, axis);

	min = std::min(pointA, pointB);
	max = std::max(pointA, pointB);
}

void  mg::SceneCollisions::ProjectOBB(mg::OBB const& obb, glm::vec2 const& axis, float& min, float& max)
{
	float projectedCenter{ glm::dot(obb.center, axis) };

	float projectedRadius{
		obb.halfExtends.x * std::abs(glm::dot(obb.axisX, axis)) +
		obb.halfExtends.y * std::abs(glm::dot(obb.axisY, axis))
	};

	min = projectedCenter - projectedRadius;
	max = projectedCenter + projectedRadius;
}

bool  mg::SceneCollisions::LineIntersectsOBB(glm::vec2 a, glm::vec2 b, mg::OBB const& obb)
{
	glm::vec2 segmentDir = glm::normalize(b - a);
	glm::vec2 perpendicular  { glm::vec2(-segmentDir.y, segmentDir.x) };

	glm::vec2 axes[3] =
	{
		obb.axisX,
		obb.axisY,
		perpendicular
	};

	for (glm::vec2 axis : axes)
	{
		float minA, maxA;
		float minB, maxB;

		ProjectSegment(a, b, axis, minA, maxA);
		ProjectOBB(obb, axis, minB, maxB);

		if (!OverLap(minA, maxA, minB, maxB))
		{
			return false;
		}
	}

	return true;
}
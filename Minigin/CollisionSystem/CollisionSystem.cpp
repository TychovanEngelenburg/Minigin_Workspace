#include "Minigin/CollisionSystem/CollisionSystem.h"
#include "Minigin/CollisionSystem/BoxCollider2D.h"
#include "Minigin/Scene/GameObject.h"

#include <glm/geometric.hpp>
#include <cassert>
#include <algorithm>
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

bool mg::SceneCollisions::CanCollide(BoxCollider2D const& A, BoxCollider2D const& B)
{
	return true;
}


//#include <iostream>
bool mg::SceneCollisions::CheckOverlap(BoxCollider2D const& A, BoxCollider2D const& B)
{
	OBB obbA{ A.GetOBB() };
	OBB obbB{ B.GetOBB() };

	int overlapping{};

	if (!OverlapOnAxis(obbA, obbB, obbA.axisX))
	{
		//++overlapping;
		return false;
	}
	if (!OverlapOnAxis(obbA, obbB, obbA.axisY))
	{
		//++overlapping;
		return false;
	}
	if (!OverlapOnAxis(obbA, obbB, obbB.axisX))
	{
		++overlapping;
		return false;
	}
	if (!OverlapOnAxis(obbA, obbB, obbB.axisY))
	{
		//++overlapping;
		return false;
	}
	//if (overlapping >= 4)
	//{
	//std::cout << "Overlapping on " << overlapping << "axis.\n";

	return true;
	//}
	//return false;
}


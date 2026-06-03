#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H
#include "Minigin/CollisionSystem/BoxCollider2D.h"

#include <vector>

namespace mg
{
	struct RaycastHit
	{
		bool Hit{ false };
		float Distance{ 0.f };
		BoxCollider2D* Collider{ nullptr };
		glm::vec2 Point{};
	};

	class SceneCollisions final
	{
	public:
		RaycastHit Raycast(glm::vec2 const& origin, glm::vec2 const& direction, float maxDistance, uint32_t layerMask = { 0xFFFFFFFF });

		void Register(BoxCollider2D* pCollider);
		void Unregister(BoxCollider2D* pCollider);

		void Update();

	private:
		static bool CanCollide(BoxCollider2D const& a, BoxCollider2D const& b);
		static bool CheckOverlap(BoxCollider2D const& a, BoxCollider2D const& b);
		static bool OverLap(float minA, float maxA, float minB, float maxB);

		static void ProjectSegment(glm::vec2 a, glm::vec2 b, glm::vec2 const& axis, float& min, float& max);
		static void ProjectOBB(mg::OBB const& obb, glm::vec2 const& axis, float& min, float& max);
		static bool LineIntersectsOBB(glm::vec2 a, glm::vec2 b, mg::OBB const& obb);

		std::vector<BoxCollider2D*> m_pColliders{};
		std::vector<std::pair<BoxCollider2D*, BoxCollider2D*>> m_pCollidingPairs{};
	};

	struct CollisionData
	{
		BoxCollider2D* pOtherCollider;
	};
};

#endif // !COLLISION_SYSTEM_H

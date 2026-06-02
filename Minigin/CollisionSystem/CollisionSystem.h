#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H
#include "Minigin/CollisionSystem/BoxCollider2D.h"

#include <vector>

namespace mg
{

	class SceneCollisions final
	{
    public:

        void Register(BoxCollider2D* pCollider);
        void Unregister(BoxCollider2D* pCollider);

        void Update();

    private:
        bool CanCollide(BoxCollider2D const& a, BoxCollider2D const& b);
        bool CheckOverlap(BoxCollider2D const& a, BoxCollider2D const& b);

        std::vector<BoxCollider2D*> m_pColliders{};
        std::vector<std::pair<BoxCollider2D*, BoxCollider2D*>> m_pCollidingPairs{};
	};

    struct CollisionData
    {
        BoxCollider2D* pOtherCollider;
    };
};

#endif // !COLLISION_SYSTEM_H

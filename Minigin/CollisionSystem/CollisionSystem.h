#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H
#include "Minigin/CollisionSystem/BoxCollider2D.h"

#include <vector>

namespace mg
{

	class SceneCollisions final
	{
    public:

        void Register(BoxCollider2D*);
        void Unregister(BoxCollider2D*);

        void Update();

    private:
        std::vector<BoxCollider2D*> m_pColliders{};
        std::vector<std::pair<BoxCollider2D*, BoxCollider2D*>> m_pCollidingPairs{};

        bool CanCollide(BoxCollider2D const& A, BoxCollider2D const& B);
        bool CheckOverlap(BoxCollider2D const& A, BoxCollider2D const& B);
	};

    struct CollisionData
    {
        BoxCollider2D* pOtherCollider;
    };
};

#endif // !COLLISION_SYSTEM_H

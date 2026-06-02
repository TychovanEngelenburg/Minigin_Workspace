#ifndef COLLISION_LAYERS_H
#define COLLISION_LAYERS_H

#include <cstdint>

enum class GameCollisionLayer : uint32_t
{
    None = 0,
    Good = 1 << 0,
    Evil = 1 << 1,
    Bullet = 1 << 2,
    All = 0xFFFFFFFF
};

struct CollisionConfig
{
    uint32_t Layer{ static_cast<uint16_t>(GameCollisionLayer::None)};
    uint32_t LayerMask{ static_cast<uint16_t>(GameCollisionLayer::All)};
};

#endif // !COLLISION_LAYERS_H
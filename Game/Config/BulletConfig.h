#ifndef BULLET_CONFIG_H
#define BULLET_CONFIG_H

#include "Game/Config/CollisionConfig.h"

#include <Minigin/Config/SpriteConfig.h>
#include <glm/vec2.hpp>

inline const mg::SpriteSheet BulletSpriteSheet{ "T_SpriteSheet_BattleTanks.png" , 4, 4 };

struct BulletConfig
{
	mg::SpriteConfig Sprite{ BulletSpriteSheet, { 0, 1 } };
	glm::vec2 ColliderSize{ 8.f, 8.f };
	CollisionConfig Collisions{};
	int Bounces{ 0 };
	float Speed{ 100.f };
	int Damage{ 1 };
};

#endif // !BULLET_CONFIG_H
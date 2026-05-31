#include "TankVisuals.h"

#include <Minigin/EngineComponents/Sprite.h>
#include <Minigin/Scene/GameObject.h>

void TankVisuals::Awake()
{
	m_pMovement = Owner()->GetComponent<TankMovement>();
	assert(m_pMovement && "EnemyBehaviour requires TankMovement");

	m_pSprite = Owner()->GetComponent<mg::Sprite>();
	assert(m_pMovement && "EnemyBehaviour requires Sprite");
	//m_currentDirection = m_pMovement->MovingDirection();
	SetSprite();
}

void TankVisuals::Update()
{
	auto dir = m_pMovement->MovingDirection();
	if (dir != m_currentDirection)
	{
		m_currentDirection = dir;
		SetSprite();
	}
}


TankVisuals::TankVisuals(mg::GameObject& owner, glm::ivec2 const& sprite)
	: Component(owner)
	, spritePos(sprite)
{

}

void TankVisuals::SetSprite()
{
	switch (m_currentDirection)
	{

		case TankMovement::Direction::Up:
		{
			m_pSprite->SetSprite({ spritePos.x + 1, spritePos.y });
			break;
		}

		case TankMovement::Direction::Down:
		{
			m_pSprite->SetSprite({ spritePos.x + 1, spritePos.y }, false, true);

			break;
		}

		case TankMovement::Direction::Left:
		{
			m_pSprite->SetSprite({ spritePos.x, spritePos.y }, true);
			break;
		}

		default:
		case TankMovement::Direction::Right:
		{
			m_pSprite->SetSprite({ spritePos.x, spritePos.y });

			break;
		}
	}
}

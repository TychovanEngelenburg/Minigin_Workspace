#include "EnemyBehaviour.h"
#include <Minigin/Scene/GameObject.h>
#include <Minigin/DeltaClock.h>
#include <iostream>

void EnemyBehaviour::Awake()
{
    m_pMovement = Owner()->GetComponent<TankMovement>();
    assert(m_pMovement && "EnemyBehaviour requires TankMovement");
}

void EnemyBehaviour::Update()
{
    m_directionTimer -= mg::DeltaClock::DeltaTime();



    if (m_directionTimer <= 0.f ||
        !m_pMovement->CanMove(m_currentDirection))
    {
        m_currentDirection = GetRandomDirection();

        m_directionTimer = 1.f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.f - 1.f)));
        std::cout << "Dir: " << static_cast<int>(m_currentDirection) << "\n";
    }

    m_pMovement->QueueMovement(m_currentDirection);
}

EnemyBehaviour::EnemyBehaviour(mg::GameObject& owner)
    : Component(owner)
{
}

TankMovement::Direction EnemyBehaviour::GetRandomDirection()
{
    auto dir{ static_cast<TankMovement::Direction>(rand() % static_cast<int>(TankMovement::Direction::End)) };


    if (dir == TankMovement::Direction::None /*||
        !m_pMovement->CanMove(dir)*/ ||
        TankMovement::IsOppositeDirection(dir, m_pMovement->MovingDirection()))
    {
        return GetRandomDirection();
    }

    return dir;
}


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

        m_directionTimer = 1.f;// +static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.f - 1.f)));
    }

    m_pMovement->QueueMovement(m_currentDirection);
}

EnemyBehaviour::EnemyBehaviour(mg::GameObject& owner)
    : Component(owner)
{
}

TankMovement::Direction EnemyBehaviour::GetRandomDirection()
{
    //auto dir{ static_cast<TankMovement::Direction>(rand() % static_cast<int>(TankMovement::Direction::End)) };


    //if (dir == TankMovement::Direction::None ||
    //    !m_pMovement->CanMove(dir) ||
    //    TankMovement::IsOppositeDirection(dir, m_pMovement->MovingDirection()))
    //{
    //    return GetRandomDirection();
    //}


    std::vector<TankMovement::Direction> validDirections{};
    TankMovement::Direction oppositeDir{};
    for (int i = 0;  i < static_cast<int>(TankMovement::Direction::End); ++i)
    {
        auto dir = static_cast<TankMovement::Direction>(i);
        if (TankMovement::IsOppositeDirection(dir, m_pMovement->MovingDirection()))
        {
            oppositeDir = dir;
            continue;
        }

        if (dir != TankMovement::Direction::None &&
            m_pMovement->CanMove(dir))
        {
            validDirections.push_back(dir);
        }
    }

    if (validDirections.empty())
    {
        return oppositeDir;
    }

    return validDirections[rand() % validDirections.size()];
}



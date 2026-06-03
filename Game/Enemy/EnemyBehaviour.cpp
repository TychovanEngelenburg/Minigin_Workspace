#include "EnemyBehaviour.h"
#include "Game/Grid/GameGrid.h"

#include <Minigin/Scene/GameObject.h>
#include <Minigin/DeltaClock.h>
#include <glm/glm.hpp>
#include <cstdlib>


void EnemyBehaviour::Awake()
{
    ChooseNextDirection(CurrentGridPos());

    m_pMovement = Owner()->GetComponent<TankMovement>();
    assert(m_pMovement && "EnemyBehaviour requires TankMovement");
}

void EnemyBehaviour::Update()
{
    if (!m_pGrid) return;

    //auto& transform = Owner()->Transform();

    if (IsAtCenterOfTile())
    {
        if (!m_hasChosenThisTile)
        {
            ChooseNextDirection(CurrentGridPos());
            m_hasChosenThisTile = true;
        }
    }
    else
    {
        m_hasChosenThisTile = false;
    }

    if (m_currentDir == TankMovement::Direction::None)
        return;

    //glm::vec2 dirVec = glm::vec2(TankMovement::DirectionToGridVector(m_currentDir));

    //glm::vec2 delta =
    //    dirVec * m_speed *
    //    static_cast<float>(mg::DeltaClock::DeltaTime());

    //transform.SetLocalPosition(transform.LocalPosition() + delta);

    m_pMovement->RequestMovement(m_currentDir);
}

EnemyBehaviour::EnemyBehaviour(mg::GameObject& owner, GameGrid& grid)
    : Component(owner)
    , m_pGrid(&grid)

{
}
void EnemyBehaviour::ChooseNextDirection(glm::ivec2 const& gridPos)
{
    auto valid = GetValidDirections(gridPos);

    if (valid.empty())
    {
        m_currentDir = TankMovement::GetOpposite(m_currentDir);
        return;
    }

    TankMovement::Direction reverse = TankMovement::GetOpposite(m_currentDir);

    std::vector<TankMovement::Direction> forward;

    for (auto d : valid)
    {
        if (d != reverse)
            forward.push_back(d);
    }

    if (!forward.empty())
    {
        m_currentDir = forward[std::rand() % forward.size()];
        return;
    }

    m_currentDir = reverse;
}

std::vector<TankMovement::Direction> EnemyBehaviour::GetValidDirections(glm::ivec2 const& p) const
{
    std::vector<TankMovement::Direction> dirs;

    if (m_pGrid->IsPath({ p.x, p.y - 1 })) dirs.push_back(TankMovement::Direction::Up);
    if (m_pGrid->IsPath({ p.x, p.y + 1 })) dirs.push_back(TankMovement::Direction::Down);
    if (m_pGrid->IsPath({ p.x - 1, p.y })) dirs.push_back(TankMovement::Direction::Left);
    if (m_pGrid->IsPath({ p.x + 1, p.y })) dirs.push_back(TankMovement::Direction::Right);

    return dirs;
}

bool EnemyBehaviour::IsAtCenterOfTile() const
{
    glm::vec2 world = Owner()->Transform().LocalPosition();
    glm::ivec2 grid = m_pGrid->WorldToGrid(world);
    glm::vec2 center = m_pGrid->GridToWorld(grid);

    return glm::distance(world, center) < 1.0f;
}

glm::ivec2 EnemyBehaviour::CurrentGridPos() const
{
    return m_pGrid->WorldToGrid(Owner()->Transform().LocalPosition());
}
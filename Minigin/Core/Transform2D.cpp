#include "Transform2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Minigin/Scene/GameObject.h"
#include <cassert>

mg::GameObject& mg::Transform2D::Object() noexcept
{
	return *m_pGameObject;
}

glm::vec2 mg::Transform2D::Forward() const noexcept
{
	return glm::vec2(std::cos(m_localRotation), std::sin(m_localRotation));
}

glm::vec2 mg::Transform2D::Right() const noexcept
{
	return glm::vec2(std::sin(m_localRotation), std::cos(m_localRotation));
}

glm::vec2 mg::Transform2D::WorldPosition() const noexcept
{
	return glm::vec2(GetWorldMatrix()[3]);
}

float mg::Transform2D::WorldRotationZ() const noexcept
{
	auto const& worldMat = GetWorldMatrix();
	return glm::degrees(std::atan2(-worldMat[1][0], worldMat[0][0]));
}

glm::vec2 mg::Transform2D::WorldScale() const noexcept
{
	auto const& worldMat = GetWorldMatrix();

	return {
		glm::length(glm::vec3(worldMat[0])),
		glm::length(glm::vec3(worldMat[1])),
	};
}



glm::vec2 const& mg::Transform2D::LocalPosition() const noexcept
{
	return m_localPosition;
}

float mg::Transform2D::LocalRotation() const noexcept
{
	return glm::degrees(m_localRotation);
}

glm::vec2 const& mg::Transform2D::LocalScale() const noexcept
{
	return m_localScale;
}



glm::mat4 const& mg::Transform2D::GetLocalMatrix() const
{
	RecalculateLocal();
	return m_localMatrix;
}

glm::mat4 const& mg::Transform2D::GetWorldMatrix() const
{
	RecalculateWorld();

	return m_worldMatrix;
}


mg::Transform2D* mg::Transform2D::Parent() const noexcept
{
	return m_pParent;
}

size_t mg::Transform2D::ChildCount() const noexcept
{
	return m_pChildren.size();
}

mg::Transform2D* mg::Transform2D::GetChildAt(size_t idx) const noexcept
{
	assert(idx < m_pChildren.size() && "Index must be in range of children!");

	if (idx >= m_pChildren.size())
	{
		return nullptr;
	}
	return m_pChildren[idx];
}

bool mg::Transform2D::HasChild(Transform2D* pChild)
{
	if (std::find(m_pChildren.begin(), m_pChildren.end(), pChild) != m_pChildren.end())
	{
		return true;
	}

	return false;
}

bool mg::Transform2D::IsInHierarchyOf(Transform2D* pAncestor)
{
	if (m_pParent != nullptr)
	{
		if (m_pParent == pAncestor)
		{
			return true;
		}

		return m_pParent->IsInHierarchyOf(pAncestor);
	}

	return false;
}

void mg::Transform2D::SetWorldPosition(glm::vec2 const& newPos)
{

	if (m_pParent)
	{
		glm::mat4 parentWorldInv = glm::inverse(m_pParent->GetWorldMatrix());
		glm::vec4 localPos = parentWorldInv * glm::vec4(newPos, 0.f, 1.f);
		m_localPosition = glm::vec2(localPos);
	}
	else
	{
		m_localPosition = newPos;
	}

	MarkDirty();
}

void mg::Transform2D::SetWorldRotation(float newDegr)
{
	if (m_pParent)
	{
		float parentWorldRot = m_pParent->WorldRotationZ();
		m_localRotation = glm::radians(newDegr - parentWorldRot);
	}
	else
	{
		m_localRotation = glm::radians(newDegr);
	}

	MarkDirty();
}

void mg::Transform2D::SetWorldScale(glm::vec2 const& newScale)
{
	if (m_pParent)
	{
		glm::vec2 parentScale = m_pParent->WorldScale();

		m_localScale.x = newScale.x / (parentScale.x == 0.f ? 1.f : parentScale.x);
		m_localScale.y = newScale.y / (parentScale.y == 0.f ? 1.f : parentScale.y);
	}
	else
	{
		m_localScale = newScale;
	}

	MarkDirty();
}

void mg::Transform2D::SetLocalPosition(glm::vec2 const& pos)
{
	m_localPosition = pos;
	MarkDirty();
}

void mg::Transform2D::SetLocalRotation(float newDegr)
{
	m_localRotation = glm::radians(newDegr);
	MarkDirty();
}

void mg::Transform2D::SetLocalScale(glm::vec2 const& scale)
{
	m_localScale = scale;
	MarkDirty();
}


void mg::Transform2D::Translate(glm::vec2 const& delta)
{
	auto forward = Forward();

	glm::vec2 rotatedDelta =
	{
		delta.x * forward.x - delta.y * forward.y,
		delta.x * forward.y + delta.y * forward.x
	};

	m_localPosition += delta;
	MarkDirty();
}

void mg::Transform2D::Rotate(float degr)
{
	m_localRotation += glm::radians(degr);
	MarkDirty();
}


void mg::Transform2D::MarkDirty()
{
	m_localDirty = true;
	MarkWorldDirty();
}

void mg::Transform2D::MarkWorldDirty()
{
	m_worldDirty = true;
	for (auto& child : m_pChildren)
	{
		child->MarkWorldDirty();
	}
}

void mg::Transform2D::SetParent(Transform2D* pParent, bool keepWorldPos)
{

	if (HasChild(pParent) || pParent == this || IsInHierarchyOf(pParent))
	{
		//assert(!pParent == this && (!m_pParent == pParent));
		return;
	}

	if (pParent == nullptr)
	{
		SetLocalPosition(WorldPosition());
		SetLocalRotation(WorldRotationZ());
		SetLocalScale(WorldScale());
	}
	else
	{
		if (keepWorldPos)
		{
			glm::mat4 newLocal = glm::inverse(pParent->GetWorldMatrix()) * GetWorldMatrix();

			m_localPosition = glm::vec2(newLocal[3]);

			glm::vec2 scale;
			scale.x = glm::length(glm::vec3(newLocal[0]));
			scale.y = glm::length(glm::vec3(newLocal[1]));
			m_localScale = scale;

			glm::mat3 rotMat;
			rotMat[0] = glm::vec3(newLocal[0]) / scale.x;
			rotMat[1] = glm::vec3(newLocal[1]) / scale.y;
			rotMat[2] = glm::vec3(0, 0, 1);
			m_localRotation = std::atan2(rotMat[0][1], rotMat[0][0]);

			MarkDirty();
		}
		else
		{
			MarkDirty();
		}
	}
	if (m_pParent)
	{
		m_pParent->RemoveChild(this);
	}

	m_pParent = pParent;

	if (m_pParent)
	{
		m_pParent->AddChild(this);
	}

	m_pGameObject->MarkActiveDirty();
}

mg::Transform2D::Transform2D(GameObject& owner)
	: m_pGameObject(&owner)
{
}

void mg::Transform2D::RecalculateLocal() const
{
	if (!m_localDirty)
	{
		return;
	}

	m_localMatrix = glm::mat4(1.f);

	m_localMatrix = glm::translate(m_localMatrix, glm::vec3(m_localPosition, 0.f));


	m_localMatrix = glm::rotate(m_localMatrix, m_localRotation, glm::vec3(0, 0, 1));

	m_localMatrix = glm::scale(m_localMatrix, glm::vec3(m_localScale.x, m_localScale.y, 1.f));

	m_localDirty = false;
}

void mg::Transform2D::RecalculateWorld() const
{
	if (!m_worldDirty)
	{
		return;
	}

	RecalculateLocal();

	if (m_pParent)
	{
		m_worldMatrix = m_pParent->GetWorldMatrix() * m_localMatrix;
	}
	else
	{
		m_worldMatrix = m_localMatrix;
	}

	m_worldDirty = false;
}

void mg::Transform2D::AddChild(Transform2D* pChild)
{
	if (!pChild)
	{
		return;
	}

	m_pChildren.emplace_back(pChild);
}

void mg::Transform2D::RemoveChild(Transform2D* pChild)
{
	std::erase(m_pChildren, pChild);
}
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject.h"

glm::vec3 mg::Transform::GetWorldPosition() const noexcept
{
	return glm::vec3(GetWorldMatrix()[3]);
}

glm::vec3 mg::Transform::GetWorldRotation() const noexcept
{
	return m_localRotation;
}

glm::vec3 mg::Transform::GetWorldScale() const noexcept
{
	auto& worldMat = GetWorldMatrix();

	return {
		glm::length(glm::vec3(worldMat[0])),
		glm::length(glm::vec3(worldMat[1])),
		glm::length(glm::vec3(worldMat[2]))
	};
}



glm::vec3 const& mg::Transform::GetLocalPosition() const noexcept
{
	return m_localPosition;
}

glm::vec3 const& mg::Transform::GetLocalRotation() const noexcept
{
	return m_localRotation;
}

glm::vec3 const& mg::Transform::GetLocalScale() const noexcept
{
	return m_localScale;
}



glm::mat4 const& mg::Transform::GetLocalMatrix() const
{
	RecalculateLocal();
	return m_localMatrix;
}

glm::mat4 const& mg::Transform::GetWorldMatrix() const
{
	RecalculateWorld();

	return m_worldMatrix;
}

void mg::Transform::SetLocalPosition(glm::vec3 const& pos)
{
	m_localPosition = pos;
	MarkDirty();
}

void mg::Transform::SetLocalRotation(glm::vec3 const& rot)
{
	m_localRotation = rot;
	MarkDirty();
}

void mg::Transform::SetLocalScale(glm::vec3 const& scale)
{
	m_localScale = scale;
	MarkDirty();
}


void mg::Transform::Translate(glm::vec3 const& delta)
{
	m_localPosition += delta;
	MarkDirty();
}

void mg::Transform::Rotate(glm::vec3 const& delta)
{
	m_localRotation += delta;
	MarkDirty();
}

void mg::Transform::SetScale(glm::vec3 const& newScale)
{
	m_localScale = newScale;
}


void mg::Transform::MarkDirty()
{
	m_localDirty = true;
	MarkWorldDirty();
}

void mg::Transform::MarkWorldDirty()
{
	m_worldDirty = true;

	for (size_t i{}; i < m_pOwner->GetChildCount(); ++i)
	{
		m_pOwner->GetChildAt(i)->GetTransform().MarkWorldDirty();
	}
}


//TODO: Add keepworldpos
void mg::Transform::SetParent(Transform* pParent, bool keepWorldPos)
{
	if (m_pParent == pParent)
	{
		return;
	}

	glm::vec3 oldWorldPos;
	if (keepWorldPos)
	{
		oldWorldPos = GetWorldPosition();
	}

	m_pParent = pParent;

	if (keepWorldPos)
	{
		glm::mat4 parentInv = glm::inverse(m_pParent->GetWorldMatrix());
		glm::vec4 localPos4 = parentInv * glm::vec4(oldWorldPos, 1.0f);
		m_localPosition = glm::vec3(localPos4);
	}
	MarkWorldDirty();
}

mg::Transform::Transform(GameObject* pOwner)
	: m_pOwner{ pOwner }
	, m_pParent{}
	, m_localPosition{}
	, m_localRotation{}
	, m_localScale{1.f, 1.f, 1.f}
	, m_localMatrix{}
	, m_worldMatrix{}
	, m_localDirty{ true }
	, m_worldDirty{ true }
{
}

void mg::Transform::RecalculateLocal() const
{
	if (!m_localDirty)
	{
		return;
	}

	m_localMatrix = glm::mat4(1.f);

	m_localMatrix = glm::translate(m_localMatrix, m_localPosition);

	m_localMatrix = glm::rotate(m_localMatrix, glm::radians(m_localRotation.x), glm::vec3(1, 0, 0));
	m_localMatrix = glm::rotate(m_localMatrix, glm::radians(m_localRotation.y), glm::vec3(0, 1, 0));
	m_localMatrix = glm::rotate(m_localMatrix, glm::radians(m_localRotation.z), glm::vec3(0, 0, 1));

	m_localMatrix = glm::scale(m_localMatrix, m_localScale);

	m_localDirty = false;
}

void mg::Transform::RecalculateWorld() const
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

#include "Transform.h"
#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>  
#include <cmath>                         
#include <numbers>                       
#include <algorithm>              

mg::GameObject* mg::Transform::GetOwner() const noexcept
{
	return m_pOwner;
}

mg::Transform* mg::Transform::GetParent() const noexcept
{
	return m_pParent;
}

int mg::Transform::GetChildCount() const noexcept
{
	return  static_cast<int>(m_children.size());
}

mg::Transform* mg::Transform::GetChildAt(size_t idx) const noexcept
{
	if (idx >= m_children.size())
	{
		return nullptr;
	}
	return m_children[idx];
}

bool mg::Transform::HasChild(Transform* child)
{

	if (std::find(m_children.begin(), m_children.end(), child) != m_children.end())
	{
		return true;
	}

	return false;
}

bool mg::Transform::IsChildOf(Transform* obj)
{
	if (m_pParent != nullptr)
	{
		if (m_pParent == obj)
		{
			return true;
		}

		return m_pParent->IsChildOf(obj);
	}

	return false;
}

const glm::vec3& mg::Transform::GetPosition()
{
	UpdateWorldPos();
	return m_worldPosition;
}

glm::vec3 const& mg::Transform::GetLocalPosition() const
{
	return m_localPosition;
}



void mg::Transform::SetParent(Transform* parent, bool keepWorldPos)
{
	if (HasChild(parent) || parent == this || m_pParent == parent)
	{
		return;
	}

	if (parent == nullptr)
	{
		SetLocalPosition(GetPosition());
	}
	else
	{
		if (keepWorldPos)
		{

			glm::vec4 localPos4 = glm::inverse(parent->GetWorldMatrix()) * glm::vec4(m_worldPosition, 1.0f);
			SetLocalPosition(glm::vec3(localPos4));
		}
		SetPositionDirty();
	}

	if (m_pParent)
	{
		m_pParent->RemoveChild(this);
	}

	m_pParent = parent;

	if (m_pParent)
	{
		m_pParent->AddChild(this);
	}
}

void mg::Transform::SetLocalPosition(glm::vec3 const& pos)
{
	m_localPosition = pos;
	SetPositionDirty();
}

void mg::Transform::SetPosition(glm::vec3 const& position)
{
	m_positionDirty = false;
	m_worldPosition = position;

	if (GetParent() == nullptr)
	{
		m_localPosition = position;
	}
	else
	{

		glm::vec4 localPos = glm::inverse(GetParent()->GetWorldMatrix()) * glm::vec4(m_worldPosition, 1.0f);
		SetLocalPosition(glm::vec3(localPos));
	}

	SetPositionDirty();
}

void mg::Transform::DestroyChildren()
{
	for (auto& child : m_children)
	{
		child->GetOwner()->Destroy();
	}
}

void mg::Transform::Translate(float x, float y, float z)
{
	Translate({ x, y, z });
}

void mg::Transform::Translate(glm::vec3 const& difference)
{
	m_localPosition += difference;
	SetPositionDirty();
}

void mg::Transform::Rotate(float degrees)
{
	m_localRotation += degrees;
}

void mg::Transform::SetScale(float scale)
{
	SetScale({ scale, scale, scale });
	SetPositionDirty();
}

void mg::Transform::SetScale(glm::vec3 const& scale)
{
	m_localScale = scale;
}

mg::Transform::Transform(GameObject& owner, glm::vec3 const& pos)
	:  m_pOwner{&owner}
	, m_positionDirty{ true }
	, m_worldPosition{}
	, m_worldRotation{}
	, m_worldScale{}
	, m_localPosition{ pos }
	, m_localRotation{}
	, m_localScale{ 1.f, 1.f, 1.f }
	, m_pParent{}
	, m_children{}
{
}



glm::mat4 mg::Transform::GetLocalMatrix() const
{
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_localPosition);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_localRotation), glm::vec3(0, 0, 1));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_localScale);

	return translation * rotation * scale;
}

glm::mat4 mg::Transform::GetWorldMatrix()
{
	UpdateWorldPos();

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_worldPosition);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_worldRotation), glm::vec3(0, 0, 1));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_worldScale);

	return translation * rotation * scale;
}

void mg::Transform::UpdateWorldPos()
{
	if (m_positionDirty)
	{
		if (GetParent() == nullptr)
		{
			m_worldPosition = m_localPosition;
			m_worldRotation = m_localRotation;
			m_worldScale = m_localScale;
		}
		else
		{
			glm::mat4 world = m_pParent->GetWorldMatrix() * GetLocalMatrix();

			m_worldPosition = glm::vec3(world[3]);

			m_worldScale.x = glm::length(glm::vec3(world[0]));
			m_worldScale.y = glm::length(glm::vec3(world[1]));
			m_worldScale.z = glm::length(glm::vec3(world[2]));

			glm::vec3 right = glm::vec3(world[0]) / m_worldScale.x;
			m_worldRotation = glm::degrees(std::atan2(right.y, right.x));
		}
	}
	m_positionDirty = false;
}

void mg::Transform::SetPositionDirty()
{
	m_positionDirty = true;
	for (auto child : m_children)
	{
		child->SetPositionDirty();
	}
}

void mg::Transform::AddChild(Transform* child)
{
	if (child == nullptr)
	{
		return;
	}

	m_children.emplace_back(child);
}

void mg::Transform::RemoveChild(Transform* child)
{
	std::erase(m_children, child);
}


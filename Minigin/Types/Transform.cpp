#include "Transform.h"
#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>  
#include <cmath>                         
#include <numbers>                       
#include <algorithm>              

dae::GameObject* dae::Transform::GetOwner() const noexcept
{
	return m_owner;
}

dae::Transform* dae::Transform::GetParent() const noexcept
{
	return m_parent;
}

int dae::Transform::GetChildCount() const noexcept
{
	return  static_cast<int>(m_children.size());
}

dae::Transform* dae::Transform::GetChildAt(size_t idx) const noexcept
{
	if (idx >= m_children.size())
	{
		return nullptr;
	}
	return m_children[idx];
}

bool dae::Transform::HasChild(Transform* child)
{

	if (std::find(m_children.begin(), m_children.end(), child) != m_children.end())
	{
		return true;
	}

	return false;
}

bool dae::Transform::IsChildOf(Transform* obj)
{
	if (m_parent != nullptr)
	{
		if (m_parent == obj)
		{
			return true;
		}

		return m_parent->IsChildOf(obj);
	}

	return false;
}

const glm::vec3& dae::Transform::GetPosition()
{
	UpdateWorldPos();
	return m_worldPosition;
}

glm::vec3 const& dae::Transform::GetLocalPosition() const
{
	return m_localPosition;
}



void dae::Transform::SetParent(Transform* parent, bool keepWorldPos)
{
	if (HasChild(parent) || parent == this || m_parent == parent)
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

	if (m_parent)
	{
		m_parent->RemoveChild(this);
	}

	m_parent = parent;

	if (m_parent)
	{
		m_parent->AddChild(this);
	}
}

void dae::Transform::SetLocalPosition(glm::vec3 const& pos)
{
	m_localPosition = pos;
	SetPositionDirty();
}

void dae::Transform::SetPosition(glm::vec3 const& position)
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

void dae::Transform::DestroyChildren()
{
	for (auto& child : m_children)
	{
		child->GetOwner()->Destroy();
	}
}

void dae::Transform::Translate(float x, float y, float z)
{
	Translate({ x, y, z });
}

void dae::Transform::Translate(glm::vec3 const& difference)
{
	m_localPosition += difference;
	SetPositionDirty();
}

void dae::Transform::Rotate(float degrees)
{
	m_localRotation += degrees;
}

void dae::Transform::SetScale(float scale)
{
	SetScale({ scale, scale, scale });
	SetPositionDirty();
}

void dae::Transform::SetScale(glm::vec3 const& scale)
{
	m_localScale = scale;
}

dae::Transform::Transform(GameObject& owner, glm::vec3 const& pos)
	:  m_owner{&owner}
	, m_positionDirty{ true }
	, m_worldPosition{}
	, m_worldRotation{}
	, m_worldScale{}
	, m_localPosition{ pos }
	, m_localRotation{}
	, m_localScale{ 1.f, 1.f, 1.f }
	, m_parent{}
	, m_children{}
{
}



glm::mat4 dae::Transform::GetLocalMatrix() const
{
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_localPosition);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_localRotation), glm::vec3(0, 0, 1));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_localScale);

	return translation * rotation * scale;
}

glm::mat4 dae::Transform::GetWorldMatrix()
{
	UpdateWorldPos();

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_worldPosition);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_worldRotation), glm::vec3(0, 0, 1));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_worldScale);

	return translation * rotation * scale;
}

void dae::Transform::UpdateWorldPos()
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
			glm::mat4 world = m_parent->GetWorldMatrix() * GetLocalMatrix();

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

void dae::Transform::SetPositionDirty()
{
	m_positionDirty = true;
	for (auto child : m_children)
	{
		child->SetPositionDirty();
	}
}

void dae::Transform::AddChild(Transform* child)
{
	if (child == nullptr)
	{
		return;
	}

	m_children.emplace_back(child);
}

void dae::Transform::RemoveChild(Transform* child)
{
	std::erase(m_children, child);
}


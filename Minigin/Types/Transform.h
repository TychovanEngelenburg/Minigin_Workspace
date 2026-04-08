#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <vector>
#include <glm/fwd.hpp>

namespace mg
{
	class GameObject;
	class Transform final
	{
	public:
		glm::vec3 GetPivotOffset() const noexcept;

		glm::vec3 GetWorldPosition() const noexcept;
		float GetWorldRotationZ() const noexcept;
		glm::vec3 GetWorldScale() const noexcept;

		glm::vec3 const& GetLocalPosition() const noexcept;
		glm::vec3 const& GetLocalRotation() const noexcept;
		glm::vec3 const& GetLocalScale() const noexcept;

		glm::mat4 const& GetLocalMatrix() const;
		glm::mat4 const& GetWorldMatrix() const;

		void SetPivotOffset(glm::vec3 const& offset);

		void SetLocalPosition(glm::vec3 const& pos);
		void SetLocalRotation(glm::vec3 const& rot);
		void SetLocalScale(glm::vec3 const& scale);

		void Translate(glm::vec3 const& difference);
		void Rotate(glm::vec3 const& difference);
		void SetScale(glm::vec3 const& newScale);

		void MarkDirty();
		void MarkWorldDirty();
		void SetParent(Transform* pParent, bool keepWorldPos = false);

		Transform(GameObject* pOwner);

		~Transform() = default;
		Transform(Transform const& other) = delete;
		Transform(Transform&& other) = delete;
		Transform& operator=(Transform const& other) = delete;
		Transform& operator=(Transform&& other) = delete;

	private:
		GameObject* m_pOwner;
		Transform* m_pParent;

		// TODO: implement offsetting the pivot point
		glm::vec3 m_pivotOffset;

		glm::vec3 m_localPosition;
		glm::vec3 m_localRotation;
		glm::vec3 m_localScale;

		mutable glm::mat4 m_localMatrix;
		mutable glm::mat4 m_worldMatrix;

		mutable bool m_localDirty;
		mutable bool m_worldDirty;

		void RecalculateLocal() const;
		void RecalculateWorld() const;
	};
}
#endif // !TRANSFORM_H

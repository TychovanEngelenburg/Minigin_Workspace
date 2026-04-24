#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <vector>
#include <glm/fwd.hpp>

namespace mg
{
	class GameObject;
	class Transform2D final
	{
	public:
		glm::vec3 GetWorldPosition() const noexcept;
		float GetWorldRotationZ() const noexcept;
		glm::vec2 GetWorldScale() const noexcept;

		glm::vec3 const& GetLocalPosition() const noexcept;
		float GetLocalRotation() const noexcept;
		glm::vec2 const& GetLocalScale() const noexcept;

		glm::mat4 const& GetLocalMatrix() const;
		glm::mat4 const& GetWorldMatrix() const;

		Transform2D* GetParent() const noexcept;
		size_t GetChildCount() const noexcept;
		Transform2D* GetChildAt(size_t idx) const noexcept;
		bool HasChild(Transform2D* pChild);
		bool IsChildOf(Transform2D* pChild);

		void SetLocalPosition(glm::vec3 const& newPos);
		void SetLocalRotation(float newDegr);
		void SetLocalScale(glm::vec2 const& newScale);

		void Translate(glm::vec3 const& delta);
		void Rotate(float degr);

		void MarkDirty();
		void MarkWorldDirty();

		void SetParent(Transform2D* pParent, bool keepRelativeWorld = false);

		Transform2D(GameObject* pOwner);

		~Transform2D() = default;
		Transform2D(Transform2D const& other) = delete;
		Transform2D(Transform2D&& other) = delete;
		Transform2D& operator=(Transform2D const& other) = delete;
		Transform2D& operator=(Transform2D&& other) = delete;

	private:
		GameObject* m_pGameObject;
		Transform2D* m_pParent;
		std::vector<Transform2D*> m_pChildren;

		glm::vec3 m_localPosition;
		float m_localRotation;
		glm::vec2 m_localScale;

		mutable glm::mat4 m_localMatrix;
		mutable glm::mat4 m_worldMatrix;

		mutable bool m_localDirty;
		mutable bool m_worldDirty;

		void RecalculateLocal() const;
		void RecalculateWorld() const;


		void AddChild(Transform2D* pChild);
		void RemoveChild(Transform2D* pChild);
	};
}
#endif // !TRANSFORM_H

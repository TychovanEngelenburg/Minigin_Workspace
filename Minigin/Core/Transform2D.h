#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/Vec2.hpp>
#include <vector>
#include <glm/mat4x4.hpp>
namespace mg
{
	class GameObject;
	class Transform2D final
	{
	public:
		GameObject& Owner() noexcept;

		glm::vec2 WorldPosition() const noexcept;
		float WorldRotationZ() const noexcept;
		glm::vec2 WorldScale() const noexcept;

		glm::vec2 const& LocalPosition() const noexcept;
		float LocalRotation() const noexcept;
		glm::vec2 const& LocalScale() const noexcept;

		glm::mat4 const& GetLocalMatrix() const;
		glm::mat4 const& GetWorldMatrix() const;

		Transform2D* Parent() const noexcept;
		size_t ChildCount() const noexcept;
		Transform2D* GetChildAt(size_t idx) const noexcept;
		bool HasChild(Transform2D* pChild);
		bool IsChildOf(Transform2D* pChild);

		void SetWorldPosition(glm::vec2 const& newPos);
		void SetWorldRotation(float newDegr);
		void SetWorldScale(glm::vec2 const& newScale);

		void SetLocalPosition(glm::vec2 const& newPos);
		void SetLocalRotation(float newDegr);
		void SetLocalScale(glm::vec2 const& newScale);

		// TODO: Translate must do actual translation matrix math (and move along the player's rotation)
		void Translate(glm::vec2 const& delta);
		void Rotate(float degr);

		void MarkDirty();
		void MarkWorldDirty();

		void SetParent(Transform2D* pParent, bool keepRelativeWorld = false);

		Transform2D(GameObject& owner);

		~Transform2D() = default;
		Transform2D(Transform2D const& other) = delete;
		Transform2D(Transform2D&& other) = delete;
		Transform2D& operator=(Transform2D const& other) = delete;
		Transform2D& operator=(Transform2D&& other) = delete;

	private:
		GameObject* m_pGameObject;
		Transform2D* m_pParent{ nullptr };
		std::vector<Transform2D*> m_pChildren;

		glm::vec2 m_localPosition{};
		float m_localRotation{};
		glm::vec2 m_localScale{ 1.f, 1.f };

		mutable glm::mat4 m_localMatrix{};
		mutable glm::mat4 m_worldMatrix{};

		mutable bool m_localDirty{ true };
		mutable bool m_worldDirty{ true };

		void RecalculateLocal() const;
		void RecalculateWorld() const;


		void AddChild(Transform2D* pChild);
		void RemoveChild(Transform2D* pChild);
	};
}
#endif // !TRANSFORM_H

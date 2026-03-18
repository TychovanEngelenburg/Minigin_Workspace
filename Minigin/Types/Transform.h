#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <vector>



// TODO: update TRS handling
namespace dae
{
	class GameObject;
	class Transform final
	{
	public:
		GameObject* GetOwner() const noexcept;

		Transform* GetParent() const noexcept;
		int GetChildCount() const noexcept;
		Transform* GetChildAt(size_t idx) const noexcept;
		bool HasChild(Transform* obj);
		bool IsChildOf(Transform* obj);
	
		glm::vec3 const& GetLocalPosition() const;
		glm::vec3 const& GetPosition();

		void SetParent(Transform* parent, bool keepWorldPos = false);
		void SetLocalPosition( glm::vec3 const& pos);
		void SetPosition( glm::vec3 const& pos);

		void DestroyChildren();

		void Translate(float x, float y, float z = 0);
		void Translate(glm::vec3 const& diff);
		void Rotate(float degrees);
		void SetScale(float scale);
		void SetScale(glm::vec3 const& diff);

		Transform(GameObject& owner, glm::vec3 const& pos = {0.f, 0.f, 0.f});

	private:
		GameObject* m_owner;

		// World data
		bool m_positionDirty;
		glm::vec3 m_worldPosition;
		float m_worldRotation;
		glm::vec3 m_worldScale;

		// Local data
		glm::vec3 m_localPosition;
		float m_localRotation;
		glm::vec3 m_localScale;

		Transform* m_parent;
		std::vector<Transform*> m_children;

		glm::mat4 GetWorldMatrix();
		glm::mat4 GetLocalMatrix() const;

		void UpdateWorldPos();

		void SetPositionDirty();
		void AddChild(Transform* child);
		void RemoveChild(Transform* child);
	};
}
#endif // !TRANSFORM_H

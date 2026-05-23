#ifndef COMPONENT_H
#define COMPONENT_H

namespace mg
{
	class GameObject;
	struct CollisionData;

	class Component
	{
	public:
		mg::GameObject* Owner() const noexcept;
		bool IsActive() const noexcept;

		void SetActive(bool isActive);

		virtual void Awake();
		virtual void Start();

		void OnCollisionEnter(CollisionData const& data);
		void OnCollisionStay(CollisionData const& data);
		void OnCollisionExit(CollisionData const& data);

		virtual void FixedUpdate();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render() const;
		
		virtual void OnApplicationQuit();

		virtual ~Component() = default;
		Component(Component const& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(Component const& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		Component(mg::GameObject& owner);

	private:
		mg::GameObject* m_pGameObject;
		bool m_active;
	};
}
#endif // !COMPONENT_H

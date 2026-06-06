#ifndef COMPONENT_H
#define COMPONENT_H

namespace mg
{
	class GameObject;
	struct CollisionData;

	class Component
	{
	public:
		mg::GameObject* Object() const noexcept;
		bool ActiveAndEnabled() const noexcept;
		bool EnabledSelf() const noexcept;

		void SetEnabled(bool isActive);

		virtual void Awake();

		virtual void OnEnable();
		virtual void OnDisable();
		virtual void Start();

		virtual void OnCollisionEnter(CollisionData const& data);
		virtual void OnCollisionStay(CollisionData const& data);
		virtual void OnCollisionExit(CollisionData const& data);

		virtual void FixedUpdate();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render() const;

		virtual ~Component() = default;
		Component(Component const& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(Component const& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		Component(mg::GameObject& owner);

	private:
		mg::GameObject* m_pGameObject;
		bool m_enabled{ true };
	};
}
#endif // !COMPONENT_H

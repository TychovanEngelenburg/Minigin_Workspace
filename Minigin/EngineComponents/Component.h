#ifndef COMPONENT_H
#define COMPONENT_H

namespace dae
{
	class GameObject;
	class Component
	{
	public:
		GameObject* GetOwner() const noexcept;
		bool IsActive() const noexcept;

		void SetActive(bool isActive);

		virtual void Start();
		virtual void FixedUpdate();
		virtual void Update();
		virtual void Render() const;
		virtual void LateUpdate();
		virtual void End();


		virtual ~Component() = default;
		Component(Component const& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(Component const& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		Component(GameObject& owner);

	private:
		GameObject* m_owner;
		bool m_active;
	};
}
#endif // !COMPONENT_H

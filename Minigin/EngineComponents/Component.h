#ifndef COMPONENT_H
#define COMPONENT_H

namespace mg
{
	class GameObject;
	class Component
	{
	public:
		mg::GameObject* Owner() const noexcept;
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
		Component(mg::GameObject& owner);

	private:
		mg::GameObject* m_pGameObject;
		bool m_active;
	};
}
#endif // !COMPONENT_H

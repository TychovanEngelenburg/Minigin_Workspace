#ifndef I_Observer_H
#define I_Observer_H

namespace mg
{
	template<typename EventType>
	class IObserver
	{
	public:
		virtual void OnNotify(EventType const& eventData) = 0;

		IObserver() = default;
		virtual ~IObserver() = default;
		IObserver(IObserver const& other) = delete;
		IObserver(IObserver&& other) = delete;
		IObserver& operator=(IObserver const& other) = delete;
		IObserver& operator=(IObserver&& other) = delete;
	};
}

#endif // !I_Observer_H

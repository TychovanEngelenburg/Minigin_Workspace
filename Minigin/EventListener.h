#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

namespace dae
{
	// Observer class. Renamed eventlistener for better readability.
	template<typename EventType>
	class EventListener
	{
	public:
		virtual void OnNotify(EventType const& eventData) = 0;

		EventListener() = default;
		virtual ~EventListener() = default;
		EventListener(EventListener const& other) = delete;
		EventListener(EventListener&& other) = delete;
		EventListener& operator=(EventListener const& other) = delete;
		EventListener& operator=(EventListener&& other) = delete;
	};
}

#endif // !EVENTLISTENER_H

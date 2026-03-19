#ifndef IEVENTLISTENER_H
#define IEVENTLISTENER_H

namespace mg
{
	// Observer class. Renamed eventlistener for better readability.
	template<typename EventType>
	class IEventListener
	{
	public:
		virtual void OnNotify(EventType const& eventData) = 0;

		IEventListener() = default;
		virtual ~IEventListener() = default;
		IEventListener(IEventListener const& other) = delete;
		IEventListener(IEventListener&& other) = delete;
		IEventListener& operator=(IEventListener const& other) = delete;
		IEventListener& operator=(IEventListener&& other) = delete;
	};
}

#endif // !IEVENTLISTENER_H

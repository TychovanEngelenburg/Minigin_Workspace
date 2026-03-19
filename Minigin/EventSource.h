#ifndef EVENTSOURE_H
#define EVENTSOURE_H
#include <vector>
#include <algorithm>

namespace mg
{
	template <typename EventType>
	class IEventListener;

	template<typename EventType>
	class EventSource final
	{
	public:
		void Notify(EventType const& eventData)
		{
			for (size_t i{}; i < m_listeners.size(); ++i)
			{
				m_listeners[i]->OnNotify(eventData);
			}
		}

		void AddListener(IEventListener<EventType>* listener)
		{
			m_listeners.push_back(listener);
		}

		void RemoveListener(IEventListener<EventType>* listener)
		{
			m_listeners.erase(std::find(m_listeners.begin(), m_listeners.end(), listener));
		}

	private:
		std::vector<IEventListener<EventType>*> m_listeners;
	};
};
#endif // !EVENTSOURE_H
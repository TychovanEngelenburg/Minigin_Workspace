#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>

namespace mg
{
	template <typename EventType>
	class IObserver;

	template<typename EventType>
	class Subject final
	{
	public:
		void Notify(EventType const& eventData)
		{
			for (size_t i{}; i < m_listeners.size(); ++i)
			{
				m_listeners[i]->OnNotify(eventData);
			}
		}

		void AddListener(IObserver<EventType>* listener)
		{
			m_listeners.push_back(listener);
		}

		void RemoveListener(IObserver<EventType>* listener)
		{
			std::erase(m_listeners, listener);
		}

	private:
		std::vector<IObserver<EventType>*> m_listeners;
	};
};
#endif // !SUBJECT_H
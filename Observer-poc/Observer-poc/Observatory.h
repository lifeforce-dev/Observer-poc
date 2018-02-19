//---------------------------------------------------------------
//
// Observatory.h
//

#pragma once

#include <algorithm>
#include <assert.h>
#include <functional>
#include <vector>

template <class T>
class Observatory
{
public:

virtual ~Observatory() {}

virtual void AddObserver(T* observer)
{
	// We're not allowed to have duplicate observers.
	assert(std::find(std::begin(m_observers), std::end(m_observers), observer) == std::end(m_observers));
	m_observers.push_back(observer);
}

virtual void RemoveObserver(T* observer)
{
	m_observers.erase(std::remove(std::begin(m_observers), std::end(m_observers), observer));
}

virtual void NotifyObservers(std::function<void(T*)> cb)
{
	std::for_each(std::cbegin(m_observers), std::cend(m_observers), cb);
}

virtual void NotifyObserversSafe(std::function<void(T*)> cb)
{
	// Will copy observers first. This is useful if observers need to be notified during destruction.
	std::vector<T*> observers = m_observers;
	std::for_each(std::cbegin(observers), std::cend(observers), cb);
}

private:
	std::vector<T*> m_observers;
};

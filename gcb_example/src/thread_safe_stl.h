#pragma once
#include <list>
#include <map>
#include <thread>
#include <mutex>

/*
template <typename T>
class thread_safe_list : public std::list<T> {
	std::recursive_mutex m_lock;
public:
	void push_back(T& value)
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);
		std::list<T>::push_back(value);
	}
	void remove(T& value)
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);
		std::list<T>::remove(value);
	}
};

template <typename KT, typename VT>
class thread_safe_map : public std::unordered_map<KT,VT> {
	std::recursive_mutex m_lock;
public:
	uint32 erase(KT& key)
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);
		return std::unordered_map<KT, VT>::erase(key);
	};
	VT& operator[](const KT& key)
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);
		return std::unordered_map<KT, VT>::operator[](key);
	}
};
*/

template <typename T>
class thread_safe_list {
	std::shared_ptr<std::recursive_mutex> m_lock;
	std::list<T> m_list;
public:
	thread_safe_list()
	{
		m_lock = std::make_shared<std::recursive_mutex>();
	}
	void lock() 
	{
		m_lock->lock();
	}
	void unlock()
	{
		m_lock->unlock();
	}
	void push_back(T& value)
	{
		std::lock_guard<std::recursive_mutex> lock(*m_lock.get());
		m_list.push_back(value);
	}
	void remove(const T& value)
	{
		std::lock_guard<std::recursive_mutex> lock(*m_lock.get());
		m_list.remove(value);
	}
	void for_each(std::function<void(T)> func)
	{
		std::lock_guard<std::recursive_mutex> lock(*m_lock.get());
		for (auto item : m_list)
		{
			func(item);
		}
	}
	T get_front()
	{
		assert(!m_list.empty());
		T tmp = m_list.front();
		m_list.pop_front();
		return tmp;
	}
	void clear()
	{
		std::lock_guard<std::recursive_mutex> lock(*m_lock.get());
		m_list.clear();
	}
	bool empty()
	{
		std::lock_guard<std::recursive_mutex> lock(*m_lock.get());
		return m_list.empty();
	}
};

template <typename KT, typename VT>
class thread_safe_map {
	std::shared_ptr<std::recursive_mutex> m_lock;
	std::unordered_map<KT, VT> m_map;
public:
	thread_safe_map()
	{
		m_lock = std::make_shared<std::recursive_mutex>();
	}
	void lock()
	{
		m_lock->lock();
	}
	void unlock()
	{
		m_lock->unlock();
	}
	uint32 erase(KT& key)
	{
		std::lock_guard<std::recursive_mutex> lock(*m_lock.get());
		return m_map.erase(key);
	};
	VT& operator[](const KT& key)
	{
		std::lock_guard<std::recursive_mutex> lock(*m_lock.get());
		return m_map[key];
	}
	void for_each(std::function<void(KT,VT)> func)
	{
		std::lock_guard<std::recursive_mutex> lock(*m_lock.get());
		for (auto item : m_map)
		{
			func(item.first, item.second);
		}
	}
};

	
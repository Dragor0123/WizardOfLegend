#pragma once

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <iostream>
#include <mutex>

using std::once_flag;

template <typename T>
class Singleton
{
public:
	static T* Get_Instance() {
		call_once(Singleton<T>::m_onceFlag, []() {
			m_pInstance = new T;
		});
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	static T* m_pInstance;
	static once_flag m_onceFlag;

	Singleton(const Singleton<T>&) = delete;
	Singleton& operator=(const Singleton<T> &) = delete;
};

template <typename T>
T* Singleton<T>::m_pInstance = nullptr;

template <typename T>
once_flag Singleton<T>::m_onceFlag;

#endif

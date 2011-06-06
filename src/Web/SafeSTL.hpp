#include "SafeSTL.h"

//=====================================================
//         线程安全的Queue的实现
//=====================================================

template<typename T>
CTSafeQueue<T>::CTSafeQueue(void)
{
}

template<typename T>
CTSafeQueue<T>::~CTSafeQueue(void)
{
}

template<typename T>
void CTSafeQueue<T>::Clear()
{
	m_hMutex.Lock();
	m_Queue.clear();
	m_hMutex.Unlock();
}

template<typename T>
void	CTSafeQueue<T>::Push( T data)
{
	m_hMutex.Lock();
	m_Queue.push( data);
	m_hMutex.Unlock();

}

template<typename T>	T	CTSafeQueue<T>::Pop( )
{
	T value;

	m_hMutex.Lock();
	value = m_Queue.front();
	m_hMutex.Unlock();

	m_Queue.pop();

	return value;
}

template< typename T>  bool CTSafeQueue<T>::Empty()
{
	m_hMutex.Lock();
	bool bEmpty = m_Queue.empty();
	m_hMutex.Unlock();

	return bEmpty;
}

template< typename T>  int CTSafeQueue<T>::Size()
{
	m_hMutex.Lock();
	int nSize = m_Queue.size();
	m_hMutex.Unlock();

	return nSize;
}

//=====================================================
//         线程安全的Vector的实现
//=====================================================

template<typename T>
CTSafeVector<T>::CTSafeVector(void)
{
}

template<typename T>
CTSafeVector<T>::~CTSafeVector(void)
{
}

template<typename T>
void	CTSafeVector<T>::Push( T data)
{
	m_hMutex.Lock();
	m_Vector.push_back( data);
	m_hMutex.Unlock();

}

template<typename T>	T	CTSafeVector<T>::Pop( )
{
	T pop_value;

	m_hMutex.Lock();
	m_hMutex.Unlock();
}

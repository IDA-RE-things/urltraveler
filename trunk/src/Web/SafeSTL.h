#pragma once

#pragma once

/***
* 线程安全的Queue实现
***/
#include <string>
#include <queue>
#include "XSync.h"

using namespace std;

template<typename T>
class CTSafeQueue
{
public:
	CTSafeQueue(void);
	~CTSafeQueue(void);

	void	Clear();
	void	Push( T data);
	T	Pop( );
	bool	Empty();

	int	Size();

private:
	deque<T>	m_Queue;				// 用以保存所有的需要处理的HTTP URL
	// Queue是先进先出的类型
	CXMutex	m_hMutex;					// 对Queue进行处理的时候所需要的
};


template<typename T>
class CTSafeVector
{
public:
	CTSafeVector(void);
	~CTSafeVector(void);

	void	Push( T data);
	T	Pop( );

private:
	vector<T>	m_Vector;				// 用以保存所有的需要处理的HTTP URL
	// Queue是先进先出的类型
	CXMutex	m_hMutex;				// 对Queue进行处理的时候所需要的
};

#include "SafeSTL.hpp"

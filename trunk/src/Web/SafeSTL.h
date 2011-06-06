#pragma once

#pragma once

/***
* �̰߳�ȫ��Queueʵ��
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
	deque<T>	m_Queue;				// ���Ա������е���Ҫ�����HTTP URL
	// Queue���Ƚ��ȳ�������
	CXMutex	m_hMutex;					// ��Queue���д����ʱ������Ҫ��
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
	vector<T>	m_Vector;				// ���Ա������е���Ҫ�����HTTP URL
	// Queue���Ƚ��ȳ�������
	CXMutex	m_hMutex;				// ��Queue���д����ʱ������Ҫ��
};

#include "SafeSTL.hpp"

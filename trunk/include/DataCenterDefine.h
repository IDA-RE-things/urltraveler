#pragma once

// DataCenter�Ķ����ͷ�ļ���Ϣ

#include "SndaBase.h"


namespace datacenter
{
	// DataCenter���⹫�����¼�
	enum E_DataCenterEventValue
	{
		EVENT_VALUE_DATACENTER_OPEN = EVENT_VALUE_DATACENTER_BEGIN,		//��������
		EVENT_VALUE_DATACENTER_HIDE,							//	���ص�ǰ������
		EVENT_VALUE_DATACENTER_CLOSE,						//	�رյ�ǰ������

		EVENT_VALUE_DATACENTER_END = EVENT_VALUE_DATACENTER_END ,			//���е��¼�����
	};

	// DataCenter�ܹ����ܶ��ⷢ�͵Ĺ㲥��Ϣ
	enum E_DataCenterMessageValue
	{
	};

	// DataCenter�ܹ������ֱ�ӵ���
	enum E_DataCenterServiceValue
	{
		SERVICE_VALUE_TEST	=	SERVICE_VALUE_DATACENTER_BEGIN,
	};

	//===========================================//
	//                   DataCenter����ʹ�õ���event�ṹ								 	  //
	//===========================================//
	struct DataCenterEvent	:	public ExtraInfo
	{
	public:
		DataCenterEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_DATACENTER;
		}
	};

};
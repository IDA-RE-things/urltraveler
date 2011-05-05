#pragma once

// DataCenter的对外的头文件信息

#include "SndaBase.h"


namespace datacenter
{
	// DataCenter对外公开的事件
	enum E_DataCenterEventValue
	{
		EVENT_VALUE_DATACENTER_OPEN = EVENT_VALUE_DATACENTER_BEGIN,		//打开主界面
		EVENT_VALUE_DATACENTER_HIDE,							//	隐藏当前主界面
		EVENT_VALUE_DATACENTER_CLOSE,						//	关闭当前主界面

		EVENT_VALUE_DATACENTER_END = EVENT_VALUE_DATACENTER_END ,			//所有的事件结束
	};

	// DataCenter能够可能对外发送的广播消息
	enum E_DataCenterMessageValue
	{
	};

	// DataCenter能够处理的直接调用
	enum E_DataCenterServiceValue
	{
		SERVICE_VALUE_TEST	=	SERVICE_VALUE_DATACENTER_BEGIN,
	};

	//===========================================//
	//                   DataCenter中所使用到的event结构								 	  //
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
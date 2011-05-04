#pragma once

#pragma warning (disable:4819)
#pragma warning (disable:4227)
#pragma warning (default:4819)
#pragma warning (default:4227)



#if defined(_WIN64)
typedef uint64 uintptr;
typedef uint32 EventValue;
typedef uint32 MessageValue;
typedef uint32 ServiceValue;
typedef uint32 uin;
typedef uint32 tid;
typedef uint64 param;
#else
typedef uint32 uintptr;
typedef uint32 EventValue;
typedef uint32 MessageValue;
typedef uint32 ServiceValue;
typedef uint32 uin;
typedef uint32 tid;
typedef uint32 param;
#endif

typedef uintptr	HTIME;
typedef uint16 ModuleIdentity;
typedef uint32 Cookie;
typedef uint32 Value;


// ��ǰ��ʹ�õ����е�ģ��ID
enum ModuleId
{
	MODULE_ID_INVALID=-1,

	MODULE_ID_CORE = 0X01,
	MODULE_ID_MAINFRAME,			//	������ģ��
	MODULE_ID_SETTING,				//	����ģ��
	MODULE_ID_DB,					//	���ݿ⴦������ģ��
	MODULE_ID_TRAYICON,				//	������ͼ�����ģ��

	MODULE_ID_END,//���һ�����߽��ж�
};

// ����ϵͳ����ʹ�õ��ĸ��ֳ���
enum 
{
	EVENT_VALUE_RANGE			= 0XFFF,
	MESSAGE_VALUE_RANGE	= 0XFFF,
	SERVICE_VALUE_RANGE		= 0XFFF,
	WM_VALUE_RANGE				= 0XFFF,
	RESOURCE_VALUE_RANGE	= 0XFFF,

	// �¼�ֵ��Χ 
	EVENT_VALUE_INVALID		= -1,
	EVENT_VALUE_BASE				= 0x10001000,
	EVENT_VALUE_END				= 0X1FFFFFFF,

	// �㲥��Ϣֵ��Χ
	MESSAGE_VALUE_INVALID	= -1,
	MESSAGE_VALUE_BASE		= 0x20001000,
	MESSAGE_VALUE_END		= 0X2FFFFFFF,

	// ֱ�ӵ���ֵ��Χ
	SERVICE_VALUE_INVALID	= -1,
	SERVICE_VALUE_BASE			= 0x30001000,
	SERVICE_VALUE_END			= 0X3FFFFFFF,

	RESOURCE_VALUE_INVALID	=-1,
	RESOURCE_VALUE_BASE		= 0x1000,
	RESOURCE_VALUE_END		=0X10000000,
};


#define EVENT_VALUE_BEGIN(X) ((X*0X1000)+EVENT_VALUE_BASE+0X01)
#define EVENT_VALUE_END(X) ((X*0X1000)+EVENT_VALUE_BASE+EVENT_VALUE_RANGE)
#define MESSAGE_VALUE_BEGIN(X) ((X*0X1000)+MESSAGE_VALUE_BASE+0X01)
#define MESSAGE_VALUE_END(X) ((X*0X1000)+MESSAGE_VALUE_BASE+MESSAGE_VALUE_RANGE)
#define SERVICE_VALUE_BEGIN(X) ((X*0X1000)+SERVICE_VALUE_BASE+0X01)
#define SERVICE_VALUE_END(X) ((X*0X1000)+SERVICE_VALUE_BASE+SERVICE_VALUE_RANGE)
#define RESOURCE_VALUE_BEGIN(X) ((X*0X1000)+RESOURCE_VALUE_BASE+0X01)
#define RESOURCE_VALUE_END(X) ((X*0X1000)+RESOURCE_VALUE_BASE+RESOURCE_VALUE_RANGE)

// �Զ�����Ϣ���
#define WM_VALUE_BEGIN(X) ((X*0X1000)+WM_USER+0X01)
#define WM_VALUE_END(X) ((X*0X1000)+WM_USER+WM_VALUE_RANGE)

#define MODULIE_ID_FROM_EVENT_VALUE(X) ((X-EVENT_VALUE_BASE-0X01)/0X1000)
#define MODULIE_ID_FROM_MESSAGE_VALUE(X) ((X-MESSAGE_VALUE_BASE-0X01)/0X1000)
#define MODULIE_ID_FROM_SERVICE_VALUE(X) ((X-SERVICE_VALUE_BASE-0X01)/0X1000)
#define MODULIE_ID_FROM_WM_VALUE(X) ((X-WM_USER-0X01)/0X1000)


__forceinline bool IsEventValue(const EventValue  x)
{
	return (((x)&0xF0000000)==0x10000000);
}
__forceinline bool IsMessageValue(const MessageValue  x)
{
	return (((x)&0xF0000000)==0x20000000);
}
__forceinline bool IsServiceValue(const ServiceValue  x)
{
	return (((x)&0xF0000000)==0x30000000);
}

enum WMValueRanges
{
	WM_VALUE_CORE_BEGIN=WM_VALUE_BEGIN(MODULE_ID_CORE),
	WM_VALUE_CORE_MAIN_LOOP_EXIT,
	WM_VALUE_CORE_END=WM_VALUE_BEGIN(MODULE_ID_CORE),


};


enum ResourceValueRanges
{
	RESOURCE_VALUE_CORE_BEGIN=RESOURCE_VALUE_BEGIN(MODULE_ID_CORE),
	RESOURCE_VALUE_CORE_END=RESOURCE_VALUE_BEGIN(MODULE_ID_CORE),

};
enum EventValueRanges
{
	EVENT_VALUE_CORE_BEGIN=EVENT_VALUE_BEGIN(MODULE_ID_CORE),//
	EVENT_VALUE_CORE_MAIN_LOOP_EXIT,
	EVENT_VALUE_SYS_END =EVENT_VALUE_END(MODULE_ID_CORE),//

	
	//�������¼�ģ��
	EVENT_VALUE_MAINFRAME_BEGIN=EVENT_VALUE_BEGIN(MODULE_ID_MAINFRAME),
	EVENT_VALUE_MAINFRAME_END=EVENT_VALUE_END(MODULE_ID_MAINFRAME),

	//���ý���ģ��
	EVENT_VALUE_SETTING_BEGIN=EVENT_VALUE_BEGIN(MODULE_ID_SETTING),
	EVENT_VALUE_SETTING_END=EVENT_VALUE_END(MODULE_ID_SETTING),

	//������ͼ�����ģ��
	EVENT_VALUE_TRAYICON_BEGIN=EVENT_VALUE_BEGIN(MODULE_ID_TRAYICON),
	EVENT_VALUE_TRAYICON_END=EVENT_VALUE_END(MODULE_ID_TRAYICON),
};

enum MessageValueRanges
{
	MESSAGE_VALUE_CORE_BEGIN=MESSAGE_VALUE_BEGIN(MODULE_ID_CORE),
	MESSAGE_VALUE_CORE_CYCLE_TRIGGER,	//	��ʱ���㲥
	MESSAGE_VALUE_CORE_BEGIN_SHOW,		//	����������֪ͨģ�����չ�֡����ģ����Ҫչ�֣����������棬����Ӧ����Ϣ
	MESSAGE_VALUE_CORE_PRELOGOUT,
	MESSAGE_VALUE_CORE_LOGOUT,
	MESSAGE_VALUE_CORE_PRE_APP_EXIT,		//	�������˳�ǰ֪ͨ����ģ����б�Ҫ��������
	MESSAGE_VALUE_CORE_END=MESSAGE_VALUE_END(MODULE_ID_CORE),
	
	//MainFrameģ��
	MESSAGE_VALUE_MAINFRAME_BEGIN=MESSAGE_VALUE_BEGIN(MODULE_ID_MAINFRAME),
	MESSAGE_VALUE_MAINFRAME_END=MESSAGE_VALUE_END(MODULE_ID_MAINFRAME),

	//����ģ��
	MESSAGE_VALUE_SETTING_BEGIN=MESSAGE_VALUE_BEGIN(MODULE_ID_SETTING),
	MESSAGE_VALUE_SETTING_END=MESSAGE_VALUE_END(MODULE_ID_SETTING),

	//������ͼ�����ģ��
	MESSAGE_VALUE_TRAYICON_BEGIN=MESSAGE_VALUE_BEGIN(MODULE_ID_TRAYICON),
	MESSAGE_VALUE_TRAYICON_END=MESSAGE_VALUE_END(MODULE_ID_TRAYICON),
};

enum ServiceValueRanges
{
	//MainFrame
	SERVICE_VALUE_MAINFRAME_BEGIN=SERVICE_VALUE_BEGIN(MODULE_ID_MAINFRAME),
	SERVICE_VALUE_MAINFRAME_END=SERVICE_VALUE_END(MODULE_ID_MAINFRAME),

	//SETTING
	SERVICE_VALUE_SETTING_BEGIN=SERVICE_VALUE_BEGIN(MODULE_ID_SETTING),
	SERVICE_VALUE_SETTING_END=SERVICE_VALUE_END(MODULE_ID_SETTING),

	//TrayIconģ��
	SERVICE_VALUE_TRAYICON_BEGIN=SERVICE_VALUE_BEGIN(MODULE_ID_TRAYICON),
	SERVICE_VALUE_TRAYICON_END=SERVICE_VALUE_END(MODULE_ID_TRAYICON),
};


struct ExtraInfo;
struct Event
{
	Event()
		:eventValue(EVENT_VALUE_INVALID),
		param0(0),
		param1(0),
		param2(0),
		param3(0),
		m_pstExtraInfo(NULL),
		srcMId(MODULE_ID_INVALID),
		desMId(MODULE_ID_INVALID)
	{
	}

	Event(EventValue  const eventValue,
		ModuleIdentity const srcMId,
		ModuleIdentity const desMId,
		param const param0=0,
		param const param1=0,
		param const param2=0,
		param const param3=0,
		ExtraInfo *extraInfo=NULL)
		:eventValue(eventValue),
		param0(param0),
		param1(param1),
		param2(param2),
		param3(param3),
		srcMId(srcMId),
		desMId(desMId),
		m_pstExtraInfo(extraInfo)
	{
	}

	EventValue  eventValue;//�¼�ֵ
	ModuleIdentity srcMId;
	ModuleIdentity desMId;
	param param0;
	param param1;
	param param2;
	union
	{
		uint32 sid;
		param param3;
	};

	ExtraInfo* m_pstExtraInfo;//������Ϣ������	
};


template<ModuleIdentity id>
struct  MakeEvent
{
	Event operator()(EventValue  const eventValue,
		ModuleIdentity const desMId,
		param const param0=0,
		param const param1=0,
		param const param2=0,
		param const param3=0,
		ExtraInfo *extraInfo=NULL)
	{
		return Event(eventValue,
			id,
			desMId,
			param0,
			param1,
			param2,
			param3,
			extraInfo);
	}
};

// ����ĸ�����Ϣ���ɸ���ģ������ȥʵ�ֶ�Ӧ��Event���ͷ��ɸ���ģ�������ͷ�
struct ExtraInfo : public Event
{
	virtual ~ExtraInfo(){} ;
};


// �㲥��Ϣ�ṹ
struct Message
{
	Message()
		:messageValue(MESSAGE_VALUE_INVALID),
		param0(0),
		param1(0),
		param2(0),
		param3(0),
		m_pstExtraInfo(NULL),
		srcMId(MODULE_ID_INVALID),
		reserved(0)
	{
	}
	Message(MessageValue  const messageValue,
		ModuleIdentity const srcMId,
		param const param0=0,
		param const param1=0,
		param const param2=0,
		param const param3=0,
		ExtraInfo   *extraInfo=NULL)
		:messageValue(messageValue),
		param0(param0),
		param1(param1),
		param2(param2),
		param3(param3),
		srcMId(srcMId),
		m_pstExtraInfo(extraInfo),
		reserved(0)
	{
	}
	MessageValue  messageValue;//��Ϣֵ
	ModuleIdentity srcMId;
	uint16 reserved;
	param param0;
	param param1;
	param param2;
	param param3;
	ExtraInfo* m_pstExtraInfo;//������Ϣ������	
};

template<ModuleIdentity id>
struct MakeMessage
{
	Message operator ()(MessageValue  const messageValue,
	param const param0=0,
	param const param1=0,
	param const param2=0,
	param const param3=0,
	ExtraInfo *extraInfo=NULL)
	{
		return Message(messageValue,
			id,
			param0,
			param1,
			param2,
			param3,
			extraInfo);
	}
};
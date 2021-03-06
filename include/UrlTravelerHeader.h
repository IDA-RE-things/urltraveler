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


// 当前所使用的所有的模块ID
enum ModuleId
{
	MODULE_ID_INVALID=-1,

	MODULE_ID_CORE = 0X01,
	MODULE_ID_MAINFRAME,			//	主界面模块
	MODULE_ID_SETTING,				//	设置模块
	MODULE_ID_TRAYICON,				//	任务栏图标管理模块
	MODULE_ID_DATACENTER,			//	数据中心模块
	MODULE_ID_LOGIN,					//	登录窗口模块 
	MODULE_ID_PLUGIN,					//	创建加载模块
	MODULE_ID_DATABASE,				//	数据库模块
	MODULE_ID_WEB,						//	Web模块，全面负责所有的HTTP请求处理
	MODULE_ID_UPDATE,					//	Web模块，全面负责所有的HTTP请求处理
	MODULE_ID_FAVMONITOR,		//  监控模块

	MODULE_ID_END,//最后一个，边界判断
};

// 定义系统中所使用到的各种常量
enum 
{
	EVENT_VALUE_RANGE		= 0XFFF,
	MESSAGE_VALUE_RANGE		= 0XFFF,
	SERVICE_VALUE_RANGE		= 0XFFF,
	WM_VALUE_RANGE			= 0XFFF,
	RESOURCE_VALUE_RANGE	= 0XFFF,

	// 事件值范围 
	EVENT_VALUE_INVALID		= -1,
	EVENT_VALUE_BASE		= 0x10001000,
	EVENT_VALUE_END			= 0X1FFFFFFF,

	// 广播消息值范围
	MESSAGE_VALUE_INVALID	= -1,
	MESSAGE_VALUE_BASE		= 0x20001000,
	MESSAGE_VALUE_END		= 0X2FFFFFFF,

	// 直接调用值范围
	SERVICE_VALUE_INVALID	= -1,
	SERVICE_VALUE_BASE		= 0x30001000,
	SERVICE_VALUE_END		= 0X3FFFFFFF,

	RESOURCE_VALUE_INVALID	= -1,
	RESOURCE_VALUE_BASE		= 0x1000,
	RESOURCE_VALUE_END		= 0X10000000,
};


#define EVENT_VALUE_BEGIN(X) ((X*0X1000)+EVENT_VALUE_BASE+0X01)
#define EVENT_VALUE_END(X) ((X*0X1000)+EVENT_VALUE_BASE+EVENT_VALUE_RANGE)
#define MESSAGE_VALUE_BEGIN(X) ((X*0X1000)+MESSAGE_VALUE_BASE+0X01)
#define MESSAGE_VALUE_END(X) ((X*0X1000)+MESSAGE_VALUE_BASE+MESSAGE_VALUE_RANGE)
#define SERVICE_VALUE_BEGIN(X) ((X*0X1000)+SERVICE_VALUE_BASE+0X01)
#define SERVICE_VALUE_END(X) ((X*0X1000)+SERVICE_VALUE_BASE+SERVICE_VALUE_RANGE)
#define RESOURCE_VALUE_BEGIN(X) ((X*0X1000)+RESOURCE_VALUE_BASE+0X01)
#define RESOURCE_VALUE_END(X) ((X*0X1000)+RESOURCE_VALUE_BASE+RESOURCE_VALUE_RANGE)

// 自定义消息起点
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

#define EVENT_VALUE(MODULE_NAME)	\
	EVENT_VALUE_##MODULE_NAME##_BEGIN=EVENT_VALUE_BEGIN(MODULE_ID_##MODULE_NAME), \
	EVENT_VALUE_##MODULE_NAME##_END=EVENT_VALUE_END(MODULE_ID_##MODULE_NAME),

enum EventValueRanges
{
	EVENT_VALUE_CORE_BEGIN=EVENT_VALUE_BEGIN(MODULE_ID_CORE),//
	EVENT_VALUE_CORE_MAIN_LOOP_EXIT,
	EVENT_VALUE_SYS_END =EVENT_VALUE_END(MODULE_ID_CORE),//

	//主界面事件模块
	EVENT_VALUE(MAINFRAME)

	//设置界面模块
	EVENT_VALUE(SETTING)

	//任务栏图标界面模块
	EVENT_VALUE(TRAYICON)

	//数据中心模块
	EVENT_VALUE(DATACENTER)

	//登录模块
 	EVENT_VALUE(LOGIN)

	//插件管理模块
 	EVENT_VALUE(PLUGIN)

	//数据库模块
 	EVENT_VALUE(DATABASE)

	//WEB模块
 	EVENT_VALUE(WEB)

	//更新模块
 	EVENT_VALUE(UPDATE)

	//FavMonitor模块
	EVENT_VALUE(FAVMONITOR)
};

#define MESSAGE_VALUE(MODULE_NAME)	\
	MESSAGE_VALUE_##MODULE_NAME##_BEGIN=MESSAGE_VALUE_BEGIN(MODULE_ID_##MODULE_NAME), \
	MESSAGE_VALUE_##MODULE_NAME##_END=MESSAGE_VALUE_END(MODULE_ID_##MODULE_NAME),


enum MessageValueRanges
{
	MESSAGE_VALUE_CORE_BEGIN=MESSAGE_VALUE_BEGIN(MODULE_ID_CORE),
	MESSAGE_VALUE_CORE_CYCLE_TRIGGER,	//	定时器广播
	MESSAGE_VALUE_CORE_BEGIN_SHOW,		//	程序启动后通知模块进行展现。如果模块需要展现，比如主界面，则响应该消息
	MESSAGE_VALUE_CORE_PRELOGOUT,
	MESSAGE_VALUE_CORE_LOGOUT,
	MESSAGE_VALUE_CORE_PRE_APP_EXIT,	//	程序在退出前通知各个模块进行必要的清理工作
	MESSAGE_VALUE_CORE_END=MESSAGE_VALUE_END(MODULE_ID_CORE),
	
	//MainFrame模块
	MESSAGE_VALUE(MAINFRAME)

	//设置模块
	MESSAGE_VALUE(SETTING)

	//任务栏图标管理模块
	MESSAGE_VALUE(TRAYICON)

	//数据中心管理模块
	MESSAGE_VALUE(DATACENTER)

	//登录管理模块
	MESSAGE_VALUE(LOGIN)

	//插件管理模块
	MESSAGE_VALUE(PLUGIN)

	//数据库管理模块
	MESSAGE_VALUE(DATABASE)

	//WEB管理模块
	MESSAGE_VALUE(WEB)

	//Update管理模块
	MESSAGE_VALUE(UPDATE)

	//FavMonitor模块
	MESSAGE_VALUE(FAVMONITOR)
};

#define SERVICE_VALUE(MODULE_NAME)	\
	SERVICE_VALUE_##MODULE_NAME##_BEGIN=SERVICE_VALUE_BEGIN(MODULE_ID_##MODULE_NAME), \
	SERVICE_VALUE_##MODULE_NAME##_END=SERVICE_VALUE_END(MODULE_ID_##MODULE_NAME),

enum ServiceValueRanges
{
	//MainFrame
	SERVICE_VALUE(MAINFRAME)

	//SETTING
	SERVICE_VALUE(SETTING)

	//TrayIcon模块
	SERVICE_VALUE(TRAYICON)

	//数据中心模块
	SERVICE_VALUE(DATACENTER)

	//登录模块
	SERVICE_VALUE(LOGIN)

	//插件管理模块
	SERVICE_VALUE(PLUGIN)

	//数据库管理模块
	SERVICE_VALUE(DATABASE)

	//WEB管理模块
	SERVICE_VALUE(WEB)

	//Update管理模块
	SERVICE_VALUE(UPDATE)

	//FavMonitor模块
	SERVICE_VALUE(FAVMONITOR)
};


struct EventExtraInfo;
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
		EventExtraInfo *extraInfo=NULL)
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

	EventValue  eventValue;//事件值
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

	EventExtraInfo* m_pstExtraInfo;//附加信息放这里	
};


template<ModuleIdentity srcId>
struct  MakeEvent
{
	Event operator()(EventValue  const eventValue,
		ModuleIdentity const desMId,
		param const param0=0,
		param const param1=0,
		param const param2=0,
		param const param3=0,
		EventExtraInfo *extraInfo=NULL)
	{
		return Event(eventValue,
			srcId,
			desMId,
			param0,
			param1,
			param2,
			param3,
			extraInfo);
	}
};

// 额外的附加消息，由各个模块自行去实现对应的Event。释放由各个模块自行释放
struct EventExtraInfo : public Event
{
	EventExtraInfo()
	{
		this->m_pstExtraInfo = this;
	}

	virtual ~EventExtraInfo(){} ;
};


struct MessageExtraInfo;
// 广播消息结构
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
		MessageExtraInfo   *extraInfo=NULL)
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
	MessageValue  messageValue;//消息值
	ModuleIdentity srcMId;
	uint16 reserved;
	param param0;
	param param1;
	param param2;
	param param3;
	MessageExtraInfo* m_pstExtraInfo;//附加信息放这里	
};

template<ModuleIdentity id>
struct MakeMessage
{
	Message operator ()(MessageValue  const messageValue,
	param const param0=0,
	param const param1=0,
	param const param2=0,
	param const param3=0,
	MessageExtraInfo *extraInfo=NULL)
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

// 额外的附加消息，由各个模块自行去实现对应的Message。释放由各个模块自行释放
struct MessageExtraInfo : public Message
{
	MessageExtraInfo()
	{
		this->m_pstExtraInfo = this;
	}

	virtual ~MessageExtraInfo(){} ;
};

struct Service
{
	Service()
	{
		serviceId = SERVICE_VALUE_INVALID;
	}

	int serviceId;
};

// 定义版本号
enum ClientVersion
{
	MAIN_VERSION	  =          0,
	SUB_VERSION		  =          1,
	PATCH_VERSION	  =          0,
	BUILD_VERSION	  =          0,

	CLIENT_VERSION		= (MAIN_VERSION<<24)+(SUB_VERSION<<16)+(PATCH_VERSION<<8) + BUILD_VERSION,
};
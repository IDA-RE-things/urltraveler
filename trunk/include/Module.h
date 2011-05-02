#pragma  once

#include "SndaBase.h"
#include "UrlTravelerHeader.h"

interface IModule;
interface IModuleFactory;
interface IModuleManager;

interface IModule
{
	//----------------------------------------------------------------------------------------
	//����: Load
	//����: ������ͨ���÷�����ģ����м���
	//����: 
	//		@param	pManager			��ģ�����ߵ�ָ��	
	//����: 
	//		������سɹ�������TRUE�����򷵻�FALSE
	//----------------------------------------------------------------------------------------
	virtual BOOL Load(IModuleManager* pManager) PURE;

	//----------------------------------------------------------------------------------------
	//����: Unload
	//����: ������ͨ���÷�����ģ�����ж��
	//����: 
	//		���ж�سɹ�������TRUE�����򷵻�FALSE
	//----------------------------------------------------------------------------------------
	virtual BOOL Unload() PURE;

	//----------------------------------------------------------------------------------------
	//����: GetModuleName
	//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
	//����: 
	//		���ж�سɹ�������TRUE�����򷵻�FALSE
	//----------------------------------------------------------------------------------------
	virtual const wchar_t* GetModuleName() PURE;

 	//----------------------------------------------------------------------------------------
	//����: GetModuleId
	//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
	//����: 
	//		���ظ�ģ���Ψһ��ID
	//----------------------------------------------------------------------------------------
	virtual uint32 const GetModuleId() PURE;

 	//----------------------------------------------------------------------------------------
	//����: ProcessEvent
	//����: ��ģ����Դ�������еĵ�Ե��¼�
	//����: 
	//		@param	evt			��Ҫ������¼�
	//----------------------------------------------------------------------------------------
	virtual void ProcessEvent(const Event& evt) PURE;

 	//----------------------------------------------------------------------------------------
	//����: ProcessMessage
	//����: ��ģ����Դ�������еĹ㲥�¼���ĳ��ģ��㲥һ����Ϣ�����߽��Ѹ���Ϣ�㲥
	//			�����е�ģ�飬ÿ��ģ�����Լ��Ƿ���Ҫ�������Ϣ�������Ҫ������ֱ�Ӵ���
	//����: 
	//		@param	msg			��Ҫ����Ĺ㲥��Ϣ
	//----------------------------------------------------------------------------------------
	virtual void ProcessMessage(const Message& msg) PURE;

 	//----------------------------------------------------------------------------------------
	//����: CallDirect
	//����: ��Event��Message��Ҫͨ��������ת��ͬ��ĳ��ģ�����ֱ�ӵ�������һ��ģ���е�
	//			����������Ҫͨ�����ߡ�
	//����: 
	//		@param	lServiceValue		����1
	//		@param	rparam			����2
	//----------------------------------------------------------------------------------------
	virtual int32 CallDirect(const ServiceValue lServiceValue, param rparam) PURE;

 	//----------------------------------------------------------------------------------------
	//����: PaybackExtraInfo
	//����: ĳ��ģ��������Զ���ĸ��ӵ�������Ҫ���������ģ�飬��ô�����Թ���һ��ExtraInfo�ṹ
	//	�������ģ��ʹ����Ϻ󣬸ýṹ���뱻�ͷţ����������ڴ�й¶���ͷű�����ģ��������ɡ�
	//	ĳ��ģ�鶼�����ṩһ��PaybackExtraInfo�ӿڣ��ͷ��Լ����Զ������������
	//����: 
	//		@param	valudId			��Ӧ��pExtraInfo��ֵ���ڲ����ݸ�ֵ���ж�Ӧ���ͷţ���ֵֻ��ģ���Լ����
	//		@param	pExtraInfo	��Ҫ�ͷŵ�ExtraInfo����
	//----------------------------------------------------------------------------------------
	virtual void PaybackExtraInfo(uint32 valudId, void* pExtraInfo) PURE;
};


#ifndef HANDLER_CALL
#define HANDLER_CALL
#endif


// Event����ӳ�亯��
#define	DECLEAR_EVENT_MAP()  \
private:   \
	typedef void (HANDLER_CALL IModule::*PEventHandler)(const Event* pEvent );     \
	typedef struct _EventHanderMapEntries   \
	{   \
		EventValue		nEventValue;   \
		PEventHandler		pfEventHandler;   \
	} EventHandlerMapEntries;   \
	static EventHandlerMapEntries m_eventMap[];  \
	\
	static const EventHandlerMapEntries* GetThisEventMap()  \
	{   \
		return &m_eventMap[0];  \
	};

#define	BEGIN_EVENT_MAP( ModuleClass ) \
	ModuleClass##::EventHandlerMapEntries ModuleClass##::m_eventMap[] ={ \

#define	ON_EVENT( event_value, event_handler)  \
	{ event_value,  (PEventHandler)&event_handler },

#define	END_EVENT_MAP() \
	{ 0, (PEventHandler)NULL}};


// ��Ϣ����ӳ�亯��
#define	DECLEAR_MESSAGE_MAP()  \
private:   \
	typedef void (HANDLER_CALL IModule::*PMessageHandler)(const Message* pMessage );     \
	typedef struct _MessageHanderMapEntries   \
	{   \
		MessageValue		nMessageValue;   \
		PMessageHandler	pfMessageHandler;   \
	} MessageHandlerMapEntries;   \
	static MessageHandlerMapEntries m_messageMap[]; \
	\
	static const MessageHandlerMapEntries* GetThisMessageMap()  \
	{   \
		return &m_messageMap[0];  \
	};

#define	BEGIN_MESSAGE_MAP( ModuleClass ) \
	ModuleClass##::MessageHandlerMapEntries ModuleClass##::m_messageMap[] ={ \

#define	ON_MESSAGE( message_value, message_handler)  \
	{ message_value,  (PMessageHandler)&message_handler },

#define	END_MESSAGE_MAP() \
	{ 0, (PMessageHandler)NULL}};



// ֱ�ӵ��ô���ӳ�亯��
#define	DECLEAR_SERVICE_MAP()  \
private:   \
	typedef int32 (HANDLER_CALL IModule::*PServiceHandler)( ServiceValue value, param wparam );    \
	typedef struct _ServiceHanderMapEntries    \
	{    \
		ServiceValue		nServiceValue;		    \
		PServiceHandler	pfServiceHandler;	\
	} ServiceHandlerMapEntries;    \
	static ServiceHandlerMapEntries m_serviceMap[];  \
	\
	static const ServiceHandlerMapEntries* GetThisServiceMap()  \
	{   \
		return &m_serviceMap[0];  \
	};


#define	BEGIN_SERVICE_MAP( ModuleClass ) \
	ModuleClass##::ServiceHandlerMapEntries ModuleClass##::m_serviceMap[] ={ \

#define	ON_SERVICE( service_value, service_handler)  \
	{ service_value, (PServiceHandler)&service_handler },

#define	END_SERVICE_MAP() \
	{ 0, (PServiceHandler)NULL}};


#define DECLARE_HANDLER_MAP()  \
	DECLEAR_EVENT_MAP()  \
	DECLEAR_MESSAGE_MAP()  \
	DECLEAR_SERVICE_MAP()


#define PROCESS_EVENT(evt) \
	EventValue ev = evt.eventValue;   \
	ASSERT( ev != EVENT_VALUE_INVALID);    \
	\
	const EventHandlerMapEntries* pEntry = GetThisEventMap();   \
	while( pEntry)   \
	{   \
		if(  pEntry->nEventValue == EVENT_VALUE_INVALID ||   \
			pEntry->nEventValue == 0)   \
			break;   \
			\
		if( pEntry->nEventValue == ev   \
			&& pEntry->pfEventHandler != NULL)   \
		{   \
			(this->*pEntry->pfEventHandler)(&evt);   \
			return;   \
		}   \
		++pEntry;   \
	}

#define PROCESS_MESSAGE(evt)  \
	MessageValue mv = msg.messageValue;  \
	ASSERT( mv != MESSAGE_VALUE_INVALID);  \
	\
	const MessageHandlerMapEntries* pEntry = GetThisMessageMap();  \
	while( pEntry)  \
	{  \
		if( pEntry->nMessageValue == MESSAGE_VALUE_INVALID  \
			|| pEntry->nMessageValue == 0)  \
			break;  \
		\
		if( pEntry->nMessageValue == mv  \
			&& pEntry->pfMessageHandler != NULL)  \
		{  \
			(this->*pEntry->pfMessageHandler)(&msg);  \
			return;  \
		}  \
		++pEntry;  \
	}

#define CALL_DIRECT(lServiceValue, rparam)  \
	ServiceValue service_value = (ServiceValue)lServiceValue;  \
	if( service_value == SERVICE_VALUE_INVALID || service_value == 0 )  \
		return -1;  \
	  \
	const ServiceHandlerMapEntries* pEntry = GetThisServiceMap();  \
	while( pEntry)  \
	{  \
		if( pEntry->nServiceValue == 0 || pEntry->nServiceValue == SERVICE_VALUE_INVALID)  \
			break;  \
		  \
		if( pEntry->nServiceValue == service_value  \
			&& pEntry->pfServiceHandler != NULL)  \
		{  \
			return (this->*pEntry->pfServiceHandler)(lServiceValue, rparam);  \
		}  \
		++pEntry;  \
	}  \
	return -1; 

//----------------------------------------------------------------------------------------
//����: IModuleFactory
//����: һ��DLL�п��԰������IModule��ͨ��IModuleFactory�����߿���֪����ǰDll�е�
//	���е�ģ�顣����IModuleFactory������Ҫ��ԭ����Ϊ�˱���Project���ࡣ
//----------------------------------------------------------------------------------------
interface IModuleFactory
{
	//----------------------------------------------------------------------------------------
	//����: QueryIModuleCounter
	//����: ���ص�ǰDll��֧�ֵ�ģ�����Ŀ
	//����: 
	//		@param	nCount			���ص�ģ����Ŀ������1��2��
	//----------------------------------------------------------------------------------------
	virtual BOOL QueryModuleCounter(uint32&  nCount) PURE;

 	//----------------------------------------------------------------------------------------
	//����: QueryIModulePoint
	//����: ���ص�ǰDll��֧�ֵ�ģ�����Ŀ
	//����: 
	//		@param	nCount			������������buf��ά�ȡ������2����������buf��ά����2
	//		@param	ppModule		ָ��һ��IModule����Ŀ�ʼλ�ã�buf��������ʵ�������
	//----------------------------------------------------------------------------------------
	virtual BOOL QueryModulePoint(uint32 nCount, IModule *&  ppModule) PURE;

	//----------------------------------------------------------------------------------------
	//����: ReleaseIModulePoint
	//����: �ͷŵ�ǰDLL�е����е�IModule
	//����: 
	//		@param	nCount			������������buf��ά�ȡ������2����������buf��ά����2
	//		@param	ppModule		ָ��һ��IModule����Ŀ�ʼλ�ã�buf��������ʵ�������
	//----------------------------------------------------------------------------------------
	virtual void ReleaseModulePoint(uint32 nCount, IModule* pModule ) PURE;
};


 //----------------------------------------------------------------------------------------
//����: IModuleManager
//����: ģ�����߽ṹ�����Զ�ģ����й����Լ�����ģ��֮����н�����
//----------------------------------------------------------------------------------------
interface IModuleManager
{
	//----------------------------------------------------------------------------------------
	//����: PushEvent
	//����: Push�������¼�
	//����: 
	//		@param	evt			��Ҫ������¼�
	//----------------------------------------------------------------------------------------
	virtual BOOL PushEvent(const Event& evt ) PURE;

	//----------------------------------------------------------------------------------------
	//����: PushMessage
	//����: ��Ҫ�㲥����Ϣ
	//����: 
	//		@param	msg			��Ҫ������¼�
	//----------------------------------------------------------------------------------------
	virtual BOOL PushMessage(const Message& msg) PURE;

	//----------------------------------------------------------------------------------------
	//����: CallService
	//����: ֱ�ӵ�������һ��ģ��ķ���
	//����: 
	//		@param	msg			��Ҫ������¼�
	//----------------------------------------------------------------------------------------
	virtual int32 CallService(const ServiceValue lServiceValue ,param rparam ) PURE;
};
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
	//		@param	lparam			����1
	//		@param	rparam			����2
	//----------------------------------------------------------------------------------------
	virtual int32 CallDirect(const param lparam, param wparam) PURE;

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

// Event����ӳ�亯��
#define	DECLEAR_EVENT_MAP()  \
private:   \
	typedef uint32 (*EventHandler)( IModule* pModule, Event* pEvent );    \
	typedef struct _EventHanderTable    \
	{    \
		EventValue		m_nEventValue;		    \
		EventHandler		m_pfEventHandler;			    \
	} EventHandlerTableEntry;    \
	static EventHandlerTableEntry m_eventTableDriven[];

// ��Ϣ����ӳ�亯��
#define	DECLEAR_MESSAGE_MAP()  \
private:   \
	typedef uint32 (*MessageHandler)( IModule* pModule, Message* pMessage );    \
	typedef struct _MessageHanderTable    \
	{    \
		MessageValue		m_nMessageValue;		    \
		MessageHandler	m_pfMessageHandler;			    \
	} MessageHandlerTableEntry;    \
	static MessageHandlerTableEntry m_messageTableDriven[];

// ֱ�ӵ��ô���ӳ�亯��
#define	DECLEAR_SERVICE_MAP()  \
private:   \
	typedef uint32 (*ServiceHandler)( IModule* pModule, ServiceValue value, param wparam );    \
	typedef struct _ServiceHanderTable    \
	{    \
		ServiceValue		m_nServiceValue;		    \
		ServiceHandler		m_pfServiceHandler;			    \
	} ServiceHandlerTableEntry;    \
	static ServiceHandlerTableEntry m_serviceTableDriven[];

#define DECLARE_HANDLER_MAP()  \
	DECLEAR_EVENT_MAP()  \
	DECLEAR_MESSAGE_MAP()  \
	DECLEAR_SERVICE_MAP()

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
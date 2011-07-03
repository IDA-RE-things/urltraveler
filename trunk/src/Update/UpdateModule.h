#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImpl.h"
#include "PlugIn.h"
#include <vector>
#include "UpdateDefine.h"
#include "UpdateWnd.h"

using namespace std;

extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}

// ��Ҫ���µ��ļ���Ϣ
typedef struct UpdateFileInfo
{
	wstring	strFileName;		//	�ļ���
	wstring	strMd5;		
	wstring	strDownloadUrl;
	wstring	strTempSavePath;	//	����֮�����ʱ���Ŀ¼

}UPDATEFILEINFO, *PUPDATEFILEINFO;

class UpdateModule : public ModuleImpl
{
	DECLEAR_SERVICE_MAP(UpdateModule)
	DECLEAR_EVENT_MAP(UpdateModule)
	DECLEAR_MESSAGE_MAP(UpdateModule)

public:
	UpdateModule();
	~UpdateModule();

	//----------------------------------------------------------------------------------------
	//����: Load
	//����: ������ͨ���÷�����ģ����м���
	//����: 
	//		@param	pManager			��ģ�����ߵ�ָ��	
	//����: 
	//		������سɹ�������TRUE�����򷵻�FALSE
	//----------------------------------------------------------------------------------------
	virtual BOOL Load(IModuleManager* pManager) ;

	//----------------------------------------------------------------------------------------
	//����: GetModuleName
	//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
	//����: 
	//		���ж�سɹ�������TRUE�����򷵻�FALSE
	//----------------------------------------------------------------------------------------
	const wchar_t* GetModuleName();

 	//----------------------------------------------------------------------------------------
	//����: GetModuleId
	//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
	//����: 
	//		���ظ�ģ���Ψһ��ID
	//----------------------------------------------------------------------------------------
	uint32 const GetModuleId();

 	//----------------------------------------------------------------------------------------
	//����: ProcessEvent
	//����: ��ģ����Դ�������еĵ�Ե��¼�
	//����: 
	//		@param	evt			��Ҫ������¼�
	//----------------------------------------------------------------------------------------
	void ProcessEvent(const Event& evt);

 	//----------------------------------------------------------------------------------------
	//����: ProcessMessage
	//����: ��ģ����Դ�������еĹ㲥�¼���ĳ��ģ��㲥һ����Ϣ�����߽��Ѹ���Ϣ�㲥
	//			�����е�ģ�飬ÿ��ģ�����Լ��Ƿ���Ҫ�������Ϣ�������Ҫ������ֱ�Ӵ���
	//����: 
	//		@param	msg			��Ҫ����Ĺ㲥��Ϣ
	//----------------------------------------------------------------------------------------
	void ProcessMessage(const Message& msg);

 	//----------------------------------------------------------------------------------------
	//����: CallDirect
	//����: ��Event��Message��Ҫͨ��������ת��ͬ��ĳ��ģ�����ֱ�ӵ�������һ��ģ���е�
	//			����������Ҫͨ�����ߡ�
	//����: 
	//		@param	lparam			����1
	//		@param	rparam			����2
	//----------------------------------------------------------------------------------------
	int32 CallDirect(const ServiceValue lServiceValue, param wparam);

protected:

	void	OnEvent_CheckUpdateInfo(Event* pEvent);
	void	OnEvent_UpdateInfoArrive(Event* pEvent);
	void	OnEvent_UpdateFileDownloaded(Event* pEvent);

protected:
	void OnMessage_CycleTrigged(Message* pMessage);

private:

	BOOL	IsHaveUpdatePackage();
	void		ProcessUpdateConfig();


protected:
	wstring	m_strUpdatePath;	//	���ص��ļ��ı���·��
	wstring	m_strUpdateXml;	//	���µ�xml�ļ�����
	wstring	m_strUpdateFileName;	//	���µ��ļ�����
	int		m_nDownloadSeqNo;
	CUpdateWnd*	m_pUpdateWnd;		
};

class UpdateModuleFactory : public ModuleFactoryImpl<UpdateModule>{};

namespace update
{
	extern UpdateModule*	g_UpdateModule;
	extern UpdateModuleFactory*	g_UpdateModuleFactory;
}
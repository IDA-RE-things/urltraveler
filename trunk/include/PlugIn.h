#pragma  once

#include "SndaBase.h"
#include "malloc.h"
#include <tchar.h>
#include <shellapi.h>
#include "tchar.h"
#include <vector>
#include "shlwapi.h"

using namespace std;

enum PlugId
{
	PLUG_ID_INVALID=-1,

	PLUG_ID_SOGOU = 0X01,	//	�ѹ�
	PLUG_ID_IE6,		    //	IE6.0,7.0
	PLUG_ID_IE9,			//	IE9.0
	PLUG_ID_MAXTHON2,		//	Maxthon 2.0
	PLUG_ID_MAXTHON3,		//	Maxthon 3.0
	PLUG_ID_360,			//	360se �����
	PLUG_ID_360EXTREME,		//	360���������
	PLUG_ID_FIREFOX,		//	FireFox
	PLUG_IN_CHROME,			//	Chrome
	PLUG_IN_TT,				//	��Ѷ��TT�����
	PLUG_IN_QQ,				//	��Ѷ��QQ�����

	PLUG_ID_END,//���һ�����߽��ж�
};


// ����ϵͳ����ʹ�õ��ĸ��ֳ���
enum 
{
	ID_VALUE_RANGE			= 0XFFF,
};


#define ID_VALUE_BEGIN(X)	(X*0X1000)
#define ID_VALUE_END(X)		((X*0X1000)+ID_VALUE_RANGE)
#define IS_ROOT_ID(id)	(id==0)



enum IDValueRanges
{
	ID_VALUE_IE6_BEGIN=ID_VALUE_BEGIN(PLUG_ID_IE6),
	ID_VALUE_IE6_END=ID_VALUE_END(PLUG_ID_IE6),

	ID_VALUE_IE9_BEGIN=ID_VALUE_BEGIN(PLUG_ID_IE9),
	ID_VALUE_IE9_END=ID_VALUE_END(PLUG_ID_IE9),

	ID_VALUE_SOGOU_BEGIN=ID_VALUE_BEGIN(PLUG_ID_SOGOU),
	ID_VALUE_SOGOU_END=ID_VALUE_END(PLUG_ID_SOGOU),

	ID_VALUE_MAXTHON2_BEGIN=ID_VALUE_BEGIN(PLUG_ID_MAXTHON2),
	ID_VALUE_MAXTHON2_END=ID_VALUE_END(PLUG_ID_MAXTHON2),

	ID_VALUE_MAXTHON3_BEGIN=ID_VALUE_BEGIN(PLUG_ID_MAXTHON3),
	ID_VALUE_MAXTHON3_END=ID_VALUE_END(PLUG_ID_MAXTHON3),

	ID_VALUE_360_BEGIN=ID_VALUE_BEGIN(PLUG_ID_360),
	ID_VALUE_360_END=ID_VALUE_END(PLUG_ID_360),

	ID_VALUE_360EXTREME_BEGIN=ID_VALUE_BEGIN(PLUG_ID_360EXTREME),
	ID_VALUE_360EXTREME_END=ID_VALUE_END(PLUG_ID_360EXTREME),

	ID_VALUE_FIREFOX_BEGIN=ID_VALUE_BEGIN(PLUG_ID_FIREFOX),
	ID_VALUE_FIREFOX_END=ID_VALUE_END(PLUG_ID_FIREFOX),

	ID_VALUE_CHROME_BEGIN=ID_VALUE_BEGIN(PLUG_IN_CHROME),
	ID_VALUE_CHROME_END=ID_VALUE_END(PLUG_IN_CHROME),

	ID_VALUE_TT_BEGIN=ID_VALUE_BEGIN(PLUG_IN_TT),
	ID_VALUE_TT_END=ID_VALUE_END(PLUG_IN_TT),

	ID_VALUE_QQ_BEGIN=ID_VALUE_BEGIN(PLUG_IN_QQ),
	ID_VALUE_QQ_END=ID_VALUE_END(PLUG_IN_QQ),
};

enum EMaxthonItemType
{
	IT_URL = 1,
	IT_FOLDER = 2,
};

// �����ղؼ����ݽṹ

// �ղؼ���ÿһ���ղؼ�¼������
typedef struct FavoriteLineData
{
	int32       nId;                //  ID
	wchar_t	    szTitle[MAX_PATH];	//	�����ı�������Ϊ�������ƣ�����ΪURL��title
	int32		nCatId;				//  ����ID
	int32		nPid;		        //	�����ID
	bool        bFolder;            //  �Ƿ���Ŀ¼��㡣�����true����ʾ��Ŀ¼��㣬�������ӽ��
	wchar_t	    szUrl[1024];		//	�ղص�URL
	time_t		nAddTimes;		    //	���ӵ�ʱ��
	time_t		nLastModifyTime;	//	����ĸ���ʱ��
	int32		nClickTimes;		//	�������
	uint32		nHashId;			//  Hash
	int32		nOrder;				//	��ǰ����ڵ�ǰ����е�˳��
	bool        bDelete;

	FavoriteLineData()
	{
		nId	=	0;
		nCatId	=	0;
		nPid	=	0;
		bFolder	=	false;
		nAddTimes	=	0;
		nLastModifyTime	=	0;
		nClickTimes	=	0;
		nHashId	=	0;
		nOrder	=	0;
		bDelete =   false;
	}

	bool operator < (const FavoriteLineData& x)const
	{
		if (nPid < x.nPid)
		{
			return true;
		}

		return false;
	}

} FAVORITELINEDATA, *PFAVORITELINEDATA; 

typedef struct HistoryLineData
{
	wchar_t	    szTitle;			//	�����ı�������Ϊ�������ƣ�����ΪURL��title
	int32		nCatId;				//	����ID
	int32		nParentCatId;		//	�����ID�����nParentCatId=0��ʾ��ǰID��Ŀ¼���
	wchar_t	    szUrl;				//	�ղص�URL
	int32		nAddTimes;			//	���ӵ�ʱ��
	int32		nLastModifyTime;	//	����ĸ���ʱ��
	int32		nClickTimes;		//	�������
	int32		nOrder;				//	��ǰ����ڵ�ǰ����е�˳��

} HISTORYLINEDATA, *PHISTORYLINEDATA;


typedef struct _winFileMem 
{
	unsigned char *pMemPointer;
	unsigned long  ulMemSize;
}winFileMem;

// ���������ӿڣ�ÿһ�������������ʵ�ָýӿ�
// �ϲ�Ӧ�ó���ͨ���ò���˽��Ӧ��������������Ϣ
interface IPlugIn
{
	//----------------------------------------------------------------------------------------
	//����: Load 
	//����: �����Load��������Ҫ�������ݵȳ�ʼ�� 
	//����: ��ʼ���ɹ�����TRUE����ʼ��ʧ�ܷ���FALSE��
	//----------------------------------------------------------------------------------------
	virtual BOOL Load() PURE;

	//----------------------------------------------------------------------------------------
	//����: UnLoad 
	//����: �����UnLoad��������Ҫ�������ݵȷ���ʼ�� 
	//����: ��ʼ���ɹ�����TRUE����ʼ��ʧ�ܷ���FALSE��
	//----------------------------------------------------------------------------------------
	virtual BOOL UnLoad() PURE;

	//----------------------------------------------------------------------------------------
	//����: IsWorked
	//����: ���ò���Ƿ��ܹ��������������û�а�װ��PlugIn��Ӧ�����������ò������Ҫ
	//	����ֻ��Ҫ����FALSE���ɡ����������ᱻ���á�
	//����:
	//      @param pszBrowserVersion[out]  �����ǰ֧�ֵ�������汾��
	//		@param nLen[in��out]           pszBrowserVersion��buffer����, ���ʵ��ʹ�ó���
	//����: TRUE��ʾ���Թ�����FALSE��ʾ���ܹ�����
	//----------------------------------------------------------------------------------------
	virtual BOOL IsWorked() PURE;

	//----------------------------------------------------------------------------------------
	//����: GetPlugInVersion
	//����: ��ȡ��ǰ����İ汾��
	//����: ����İ汾�ţ�ͨ��Ϊһ������
	//----------------------------------------------------------------------------------------
	virtual int32 GetPlugInVersion() PURE;

	//----------------------------------------------------------------------------------------
	//����: GetBrowserName
	//����: ��ȡ�ò����Ӧ������������ƺͰ汾
	//----------------------------------------------------------------------------------------
	virtual const wchar_t* GetBrowserName() PURE;

	//----------------------------------------------------------------------------------------
	//����: GetBrowserIcon
	//����: ��ȡ�ò����Ӧ���������ͼ��
	//----------------------------------------------------------------------------------------
	virtual HICON GetBrowserIcon() PURE;

	//----------------------------------------------------------------------------------------
	//����: GetInstallPath
	//����: ��ȡ�����Ӧ��������İ�װĿ¼
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetInstallPath() PURE;

	//----------------------------------------------------------------------------------------
	//����: GetFavoriteDataPath
	//����: ��ȡ������ղؼж�Ӧ���ļ������ļ��е�·��
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetFavoriteDataPath() PURE;

	//----------------------------------------------------------------------------------------
	//����: GetHistoryDataPath
	//����: ��ȡ������ղؼе���ʷ���ݶ�Ӧ���ļ������ļ���
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetHistoryDataPath() PURE;

	//----------------------------------------------------------------------------------------
	//����: ExportFavoriteData
	//����: ������ǰ����������е��ղؼ�����
	//����: 
	//		@param	pData			�������ղؼ���������
	//		@param	nDataNum		�������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ExportFavoriteData(PFAVORITELINEDATA pData, int32& nDataNum) PURE;

	//----------------------------------------------------------------------------------------
	//����: ImportFavoriteData
	//����: ����ǰ�ļ�¼���뵽�������
	//����: 
	//		@param	pData			��Ҫ����ĵ��ղؼ���������
	//		@param	nDataNum		��Ҫ������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ImportFavoriteData(PFAVORITELINEDATA pData, int32 nDataNum) PURE;

	//----------------------------------------------------------------------------------------
	//����: ExportHistoryData
	//����: ������ʷ�����¼
	//����: 
	//		@param	pData			�������ղؼ���������
	//		@param	nDataNum		�������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ExportHistoryData(PHISTORYLINEDATA pData, int32& nDataNum) PURE;

	//----------------------------------------------------------------------------------------
	//����: ImportHistoryData
	//����: ����ʷ��¼��¼���뵽�������
	//����: 
	//		@param	pData			��Ҫ����ĵ��ղؼ���������
	//		@param	nDataNum		��Ҫ������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------

	virtual BOOL ImportHistoryData(PHISTORYLINEDATA pData, int32 nDataNum) PURE;
	//----------------------------------------------------------------------------------------
	//����: GetHistoryCount
	//����: ����ʷ��¼��¼���뵽�������
	//����: 
	//      ������ʷ��¼����
	//----------------------------------------------------------------------------------------
	virtual int32 GetHistoryCount() PURE;

	//----------------------------------------------------------------------------------------
	//����: GetFavoriteCount
	//����: ��ȡ��������ղ���ַ������ 
	//����:
	//      �ط��ղ���ַ����
	//----------------------------------------------------------------------------------------
	virtual int32 GetFavoriteCount() PURE;
};

class IPlugInFactory;

typedef IPlugInFactory *  (*GetPlugInFactoryFunc)();
typedef void  (*ReleasePlugInFactoryFunc)();

typedef struct _PlugInInfo
{
	wchar_t			wszPlugInName[MAX_PATH];
	HMODULE			hModule;
	std::vector<IPlugIn*>	pvPlugIn;		//	���������

	GetPlugInFactoryFunc	pGetPlugInFactoryFunc;
	ReleasePlugInFactoryFunc	pReleasePlugInFactoryFunc;

}PLUGININFO, *PPLUGININFO;

class PlugInImp : public IPlugIn
{
public:

	//----------------------------------------------------------------------------------------
	//����: Load 
	//����: �����Load��������Ҫ�������ݵȳ�ʼ�� 
	//����: ��ʼ���ɹ�����TRUE����ʼ��ʧ�ܷ���FALSE��
	//----------------------------------------------------------------------------------------
	virtual BOOL Load()
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//����: UnLoad 
	//����: �����UnLoad��������Ҫ�������ݵȷ���ʼ�� 
	//����: ��ʼ���ɹ�����TRUE����ʼ��ʧ�ܷ���FALSE��
	//----------------------------------------------------------------------------------------
	virtual BOOL UnLoad()
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//����: IsWorked
	//����: ���ò���Ƿ��ܹ��������������û�а�װ��PlugIn��Ӧ�����������ò������Ҫ
	//	����ֻ��Ҫ����FALSE���ɡ����������ᱻ���á�
	//����:
	//      @param pszBrowserVersion[out]  �����ǰ֧�ֵ�������汾��
	//		@param nLen[in��out]           pszBrowserVersion��buffer����, ���ʵ��ʹ�ó���
	//����: TRUE��ʾ���Թ�����FALSE��ʾ���ܹ�����
	//----------------------------------------------------------------------------------------
	virtual BOOL IsWorked()
	{
		wchar_t* pszInstallPath = GetInstallPath();

		if( pszInstallPath == NULL)
			return FALSE;

		// ����ļ��Ƿ����
		if( PathFileExists(pszInstallPath) == FALSE)
		{
			free(pszInstallPath);
			return FALSE;
		}

		free(pszInstallPath);
		return TRUE;
	}

	//----------------------------------------------------------------------------------------
	//����: GetPlugInVersion
	//����: ��ȡ��ǰ����İ汾��
	//����: ����İ汾�ţ�ͨ��Ϊһ������
	//----------------------------------------------------------------------------------------
	virtual int32 GetPlugInVersion()
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------
	//����: GetBrowserName
	//����: ��ȡ�ò����Ӧ������������ƺͰ汾
	//----------------------------------------------------------------------------------------
	virtual const wchar_t* GetBrowserName()
	{
		return L"";
	}

	//----------------------------------------------------------------------------------------
	//����: GetBrowserIcon
	//����: ��ȡ�ò����Ӧ���������ͼ��
	//----------------------------------------------------------------------------------------
	virtual HICON GetBrowserIcon()
	{
		wchar_t *pszBrowserPath = GetInstallPath();
		HICON    hIcon = NULL;

		if (_wcsicmp(pszBrowserPath, _T("")) == 0)
		{
			free(pszBrowserPath);
			return NULL;
		}

		hIcon = ExtractIcon(GetModuleHandle(NULL), GetInstallPath(), 0);
		free(pszBrowserPath);

		return hIcon;
	}

	//----------------------------------------------------------------------------------------
	//����: GetInstallPath
	//����: ��ȡ�����Ӧ��������İ�װĿ¼
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetInstallPath()
	{
		return L"";
	}

	//----------------------------------------------------------------------------------------
	//����: GetFavoriteDataPath
	//����: ��ȡ������ղؼж�Ӧ���ļ������ļ��е�·��
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetFavoriteDataPath()
	{
		return L"";
	}

	//----------------------------------------------------------------------------------------
	//����: GetHistoryDataPath
	//����: ��ȡ������ղؼе���ʷ���ݶ�Ӧ���ļ������ļ���
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetHistoryDataPath()
	{
		return L"";
	}

	//----------------------------------------------------------------------------------------
	//����: ExportFavoriteData
	//����: ������ǰ����������е��ղؼ�����
	//����: 
	//		@param	pData			�������ղؼ���������
	//		@param	nDataNum		�������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ExportFavoriteData(PFAVORITELINEDATA pData, int32& nDataNum)
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//����: ImportFavoriteData
	//����: ����ǰ�ļ�¼���뵽�������
	//����: 
	//		@param	pData			��Ҫ����ĵ��ղؼ���������
	//		@param	nDataNum		��Ҫ������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ImportFavoriteData(PFAVORITELINEDATA pData, int32 nDataNum)
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//����: ExportHistoryData
	//����: ������ʷ�����¼
	//����: 
	//		@param	pData			�������ղؼ���������
	//		@param	nDataNum		�������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ExportHistoryData(PHISTORYLINEDATA pData, int32& nDataNum)
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//����: ImportHistoryData
	//����: ����ʷ��¼��¼���뵽�������
	//����: 
	//		@param	pData			��Ҫ����ĵ��ղؼ���������
	//		@param	nDataNum		��Ҫ������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ImportHistoryData(PHISTORYLINEDATA pData, int32 nDataNum)
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//����: GetHistoryCount
	//����: ����ʷ��¼��¼���뵽�������
	//����: 
	//      ������ʷ��¼����
	//----------------------------------------------------------------------------------------
	virtual int32 GetHistoryCount()
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------
	//����: GetFavoriteCount
	//����: ��ȡ��������ղ���ַ������ 
	//����:
	//      �ط��ղ���ַ����
	//----------------------------------------------------------------------------------------
	virtual int32 GetFavoriteCount()
	{
		return 0;
	}

	void ReplaceSingleQuoteToDoubleQuote(wchar_t *pszOri)
	{
		int32 nLen = _tcslen(pszOri);

		if (pszOri == NULL || nLen == 0)
		{
			return;
		}

		for (int i = 0; i < nLen; i++)
		{
			if (pszOri[i] == '\'')
			{
				pszOri[i] = '\"';
			}
		}
	}
};

//----------------------------------------------------------------------------------------
//����: IPlugInFactory
//����: һ��DLL�п��԰������IPlugIn��ͨ��IPlugInFactory�����߿���֪����ǰDll�е�
//	���е�ģ�顣����IPlugInFactory������Ҫ��ԭ����Ϊ�˱���Project���ࡣ
//----------------------------------------------------------------------------------------
interface IPlugInFactory
{
	//----------------------------------------------------------------------------------------
	//����: QueryIPlugInCounter
	//����: ���ص�ǰDll��֧�ֵ�ģ�����Ŀ
	//����: 
	//		@param	nCount			���ص�ģ����Ŀ������1��2��
	//----------------------------------------------------------------------------------------
	virtual BOOL QueryPlugInCounter(uint32&  nCount) PURE;

	//----------------------------------------------------------------------------------------
	//����: QueryIPlugInPoint
	//����: ���ص�ǰDll��֧�ֵ�ģ�����Ŀ
	//����: 
	//		@param	nCount			������������buf��ά�ȡ������2����������buf��ά����2
	//		@param	ppPlugIn		ָ��һ��IPlugIn����Ŀ�ʼλ�ã�buf��ʵ�����ṩ����䡣
	//								�ⲿͨ�������ָ��
	//----------------------------------------------------------------------------------------
	virtual BOOL QueryPlugInPoint(uint32 nCount, IPlugIn *&  ppPlugIn) PURE;

	//----------------------------------------------------------------------------------------
	//����: ReleaseIPlugInPoint
	//����: �ͷŵ�ǰDLL�е����е�IPlugIn
	//����: 
	//		@param	nCount			������������buf��ά�ȡ������2����������buf��ά����2
	//		@param	ppPlugIn		ָ��һ��IPlugIn����Ŀ�ʼλ�ã�buf��������ʵ�������
	//----------------------------------------------------------------------------------------
	virtual void ReleasePlugInPoint(uint32 nCount, IPlugIn* pPlugIn ) PURE;
};

// 
template <typename T>
class PlugInFactoryImpl :public  IPlugInFactory
{
public:
	PlugInFactoryImpl(){}
	~PlugInFactoryImpl()
	{
	}

	virtual BOOL QueryPlugInCounter(uint32 & counter)
	{
		counter=1;
		return TRUE;
	}

	virtual BOOL QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn)
	{
		PAssert_ReturnWithValue(1 == counter, false);

		IPlugIn** ptrPlugIn = &pPlugIn;
		ptrPlugIn[0] = (IPlugIn *)&m_stPlugIn;

		return FALSE;
	}

	virtual void ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn)
	{
	}

protected:

	T  m_stPlugIn;
};

#pragma once

/****************************************************************************
* ClassName:   CResponseParser												*
* Author:	   Zhang Z.Q													*
* Using:	   该类负责对响应的报文数据进行XML解析，然后将其保存到数据中心	*
*																			*
* Verion:																	*
*		Created			2007,12,12											*
*****************************************************************************/
#include "WebDefine.h"
#include <string>
#include <map>
#include "ximage.h"
#include "httpmanager.h"
#include "HttpContext.h"

using namespace std;
using namespace web;

#define	RET_ERROR_XML_FILE		1			// 返回的XML文件错误，无法解析
#define	RET_ERROR_RESPONSE		2			// 发生错误的时候服务器的返回报文，通常是执行不成功
#define RET_ERROR_NO_WEBDATA	3			// 解析过程中没有指定解析数据仓库
#define RET_ERROR_NO_HANDLER	4			// 没有找到对应的解析函数
#define RET_INNER_USED			5			// 该解析过程内部使用，不需要对外提供
											// 比如对于用户形象照的获取就应该返回该值
#define RET_SUCCESS				7			// 执行成功，数据解析后放入到数据中心


class CWebData;
class CRequestManager;
class CResponseParser
{
public:
	CResponseParser(void);
	~CResponseParser(void);

	static string	ConvertCharVector( CHAR_VECTOR* pVector);

	void			SetWebData( CWebData* pWebData);
	CWebData*	GetWebData( );

	void				SetReuqestManager( CRequestManager*	pRequestManager);
	CRequestManager*	GetRequestManager() { return m_pRequestManager;}


	/**
	 * Function		根据给定的EventValue值对返回的响应进行解析
	 * @param		nEventValue 对应的事件值
	 * @param		lpszContentData 对应的内容
	 * @param		dwContentSize 读取的报文的长度
	 * @param		nEltsNum 对于获取好友列表而言，需要返回一个值，只是当前的好友的数目，以便调用者分配内存空间
	 * @return 
	 **/
	void	ParseResponse( HTTPCONTEXT* pContext);

private:
	//==========================================================
	//              内部使用的辅助函数
	//==========================================================
	//static void	ParseGetUpdateConfigXml( CResponseParser* pParser, HTTPCONTEXT* pContext);

private:

	CRequestManager*	m_pRequestManager;		// 处理器在获取到图片URL的时候需要调用CReuqestManager，
												// 请求其返回图片数据
	CWebData*			m_pWebData;				// 数据中心

	// 处理句柄指针
	typedef void (*ParseHandler)( CResponseParser* pParser, HTTPCONTEXT* pContext );
	typedef struct _HanderTable
	{
		EventValue		m_nEventValue;		//	对应的事件值
		ParseHandler	m_hHandler;			//	处理句柄

	} HandlerTableEntry;
	static HandlerTableEntry m_tableDriven[];
};

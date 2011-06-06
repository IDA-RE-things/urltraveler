#include "httpmanager.h"
#include "HttpClient.h"
#include "shlwapi.h"
#include "Log.h"
#include "StringHelper.h"
#include "WebModule.h"
#include "XSync.h"
#include "assert.h"

#pragma comment(lib,"shlwapi.lib")

HANDLE get_file_handle( wstring wstrFileName)
{
	// 创建文件，并写入数据
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	return ::CreateFile(wstrFileName.c_str(), GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}


INT32 handle_init(INT32 taskid, const TaskInfo* taskinfo)
{
	return 0;
}

INT32 handle_headcomplete(INT32 taskid, const TaskInfo* taskinfo)
{
	CHttpClient* pHttpClient = CHttpClient::Instance();
	int32 nSeqNo = pHttpClient->GetSeqNo(taskid);

	if( nSeqNo != -1)
	{
		HTTPCONTEXT* pContext = pHttpClient->GetContext( nSeqNo);

		// 不是边读边写类型
		if( !pContext || (pContext && pContext->eReqType == ONCE_BUFFER ))
			return -1;

		pContext->nContentLength = taskinfo->contentLength;
		return 0;
	}

	return -1;
}

INT32 handle_process(INT32 taskid, const TaskInfo* taskinfo)
{
	static int threadid = GetCurrentThreadId();
	if(threadid != GetCurrentThreadId())
	{
		ASSERT(0);
	}

	if( taskinfo == NULL )
	{
		return -1;
	}

	CHttpClient* pHttpClient = CHttpClient::Instance();
	int32 nSeqNo = pHttpClient->GetSeqNo(taskid);

	if( nSeqNo != -1)
	{
		HTTPCONTEXT* pContext = pHttpClient->GetContext( nSeqNo);
		if( !pContext )
			return -1;

		pContext->uResult = SUCCESS;
		pContext->nHttpCode = taskinfo->httpCode;
		pContext->nErrorCode = taskinfo->errcode;

		//  get方法的时候会自动把头部剔除 
		if( taskinfo->taskType == TASK_DOWNLOAD)
		{
			pContext->nFinishedLength = taskinfo->doneSize;
			pContext->nContentLength = taskinfo->contentLength;
		}
		// 上载的时候需要剔除上载的头部
		else if( taskinfo->taskType == TASK_UPLOAD || taskinfo->taskType == TASK_BINARY)
		{
			pContext->nFinishedLength = taskinfo->doneSize- taskinfo->htrequest->GetAllHdrLen();
			pContext->nContentLength = taskinfo->contentLength - taskinfo->htrequest->GetAllHdrLen();
		}
		// 对于Query方法，它的DoneSize也是剔除了头之后的长度，其余的为下载，剔除下载头部
		else
		{
			pContext->nFinishedLength = taskinfo->doneSize;
			pContext->nContentLength = taskinfo->contentLength;
		}

		pContext->nSpeed	= taskinfo->transferSpeed;
		pContext->nRemainedTime	= taskinfo->remainTime;

		if( taskinfo->taskType == TASK_UPLOAD )
		{
		}

		// 将接收到的数据写入文件
		if( pContext->eReqType == ONCE_FILE)
		{
			if( pContext->hFileHandle == 0)
				pContext->hFileHandle = get_file_handle( pContext->wstrWritedFileName);

			string strCurrentBuffer;
			strCurrentBuffer.append( (*(taskinfo->respData)).begin(), (*(taskinfo->respData)).end());

			DWORD dwWrite = 0;
			BOOL bRet = ::WriteFile( pContext->hFileHandle, strCurrentBuffer.c_str(), strCurrentBuffer.size(), &dwWrite, NULL);   //写入
			// 写入磁盘失败
			if( bRet == FALSE)
			{
				pContext->uResult = FAILED;
				pContext->nErrorCode = ERR_TASK_WRITE_FILE_ERROR;

				pHttpClient->Cleanup( pContext);
			}
		}

		// 对于读取到的数据立即返回给上层应用
		else if( pContext->eReqType == FRAG_BUFFER)
		{
			pContext->strContentData.clear();
			pContext->strContentData.append( (*(taskinfo->respData)).begin(), (*(taskinfo->respData)).end());

			// param0用以表示是否是中间状态回复，而不是最后的回复
			pContext->uParam0 = 0;

			pHttpClient->Cleanup( pContext);
		}                        

		return 0;
	}

	return -1;
}

INT32 handle_finish(INT32 taskid, const TaskInfo* taskinfo)
{
	static int threadid = GetCurrentThreadId();
	if(threadid != GetCurrentThreadId())
	{
		ASSERT(0);
	}

	if( taskinfo == NULL || taskinfo->respData == NULL)
	{
		return -1;
	}

	CHttpClient* pHttpClient = CHttpClient::Instance();
	int32 nSeqNo = pHttpClient->GetSeqNo(taskid);

	if( nSeqNo != -1)
	{
		HTTPCONTEXT* pContext = pHttpClient->GetContext( nSeqNo);
		if( pContext)
		{
			pContext->uResult = SUCCESS;
			pContext->nHttpCode = taskinfo->httpCode;
			pContext->nErrorCode = taskinfo->errcode;

			//  get方法的时候会自动把头部剔除 
			if( taskinfo->taskType == TASK_DOWNLOAD)
			{
				pContext->nFinishedLength = taskinfo->doneSize;
				pContext->nContentLength = taskinfo->contentLength;

			}
			// 上载的时候需要剔除上载的头部
			else if( taskinfo->taskType == TASK_UPLOAD || taskinfo->taskType == TASK_BINARY)
			{
				pContext->nFinishedLength = taskinfo->doneSize- taskinfo->htrequest->GetAllHdrLen();
				pContext->nContentLength = taskinfo->contentLength - taskinfo->htrequest->GetAllHdrLen();
			}
			// 对于Query方法，它的DoneSize也是剔除了头之后的长度，其余的为下载，剔除下载头部
			else
			{
				pContext->nFinishedLength = taskinfo->doneSize;
				if( pContext->nFinishedLength < 0)
					pContext->nFinishedLength = 0;

				pContext->nContentLength = taskinfo->contentLength;
				if( pContext->nContentLength < 0)
					pContext->nContentLength = 0;
			}

			pContext->nSpeed	= taskinfo->transferSpeed;
			pContext->nRemainedTime	= taskinfo->remainTime;

			if( taskinfo->taskType == TASK_UPLOAD && taskinfo->respData->size())
			{
			}

			// 需要立即写入到文件中
			if( pContext->eReqType == ONCE_FILE)
			{
				if( pContext->hFileHandle == 0)
					pContext->hFileHandle = get_file_handle( pContext->wstrWritedFileName);

				string strCurrentBuffer;
				strCurrentBuffer.append( (*(taskinfo->respData)).begin(), (*(taskinfo->respData)).end());

				DWORD dwWrite = 0 ;
				BOOL bRet = ::WriteFile( pContext->hFileHandle, strCurrentBuffer.c_str(), strCurrentBuffer.size(), &dwWrite, NULL);   //写入
				CloseHandle( pContext->hFileHandle);
				pContext->hFileHandle = 0;

				// 写入磁盘失败
				if( bRet == FALSE)
				{
					pContext->uResult = FAILED;
					pContext->nErrorCode = ERR_TASK_WRITE_FILE_ERROR;

					pHttpClient->Cleanup( pContext);
				}
			}

			// 全部读取，最后一次性处理
			else
			{
				pContext->strContentData.clear();
				pContext->strContentData.append( (*(taskinfo->respData)).begin(), (*(taskinfo->respData)).end());
				pContext->uParam2 = 1;
			}

			pHttpClient->Cleanup( pContext);
		}

		return 0;
	}

	return -1;
}

INT32 handle_error(INT32 taskid, const TaskInfo* taskinfo)
{
	static int threadid = GetCurrentThreadId();
	if(threadid != GetCurrentThreadId())
	{
		ASSERT(0);
	}

	CHttpClient* pHttpClient = CHttpClient::Instance();
	int32 nSeqNo = pHttpClient->GetSeqNo(taskid);
	if( nSeqNo != -1)
	{
		HTTPCONTEXT* pContext = pHttpClient->GetContext( nSeqNo);
		if( pContext)
		{
			pContext->uResult = FAILED;
			pContext->nHttpCode = taskinfo->httpCode;
			pContext->nErrorCode = taskinfo->errcode;
			pContext->nContentLength = taskinfo->contentLength;
			if( taskinfo->respData != NULL && (*(taskinfo->respData)).size() > 0)
			{
				pContext->strContentData.append( (*(taskinfo->respData)).begin(), (*(taskinfo->respData)).end());
			}
			else
			{
			}

			pHttpClient->Cleanup( pContext);
		}

		return 0;
	}

	return -1;
}

#include "stdafx.h"
#include "task.h"
#ifndef	 HTTPTRANSFER_RELEASE
#include "log.h"
#else
#include "log_stun.h"
#endif

#include "Url.h"
#include "httptransfer.h"
#include "Utils.h"
#include "HttpCli.h"


extern int ret_to_err(int retcode);

void TTask::init()
{
	redirectionTimes = 0;
	connectTimes = 0;
	shouldGetFileInfo = true;
	memset(&taskInfo, 0x0, sizeof(taskInfo));
	taskInfo.addTime = (int)time(NULL);
	taskInfo.contentLength = -1;
	taskInfo.taskType = task_type;
	handle_init = NULL;
	handle_process = NULL;
	handle_finish = NULL;
	handle_error = NULL;
	timeoutlimit = 0;
	last_active_time = 0;

	startSize = 0;			//���ζ�ȡhttp��Ӧ�����ʼ�ֽڣ���Ϊ0���������
	endSize = 0;			//���ζ�ȡhttp��Ӧ��Ľ����ֽڣ���Ϊ0���������
	dataClean = 0;			//�Ƿ���ÿ�λص�֮�����յ���������գ��ѽ�ʡ�ڴ�

	taskState = TSK_UNKNOWN;
	errcode = 0;

	del = false;

	requestBuffer = NULL;
	
	requestBufferSize = 0;
	requestSentSize = 0;
	postDataSize = 0;

	respHeadCompleted = false;
	respContentCompleted = false;

	respData = &respDataContent;
	dataChanged = false;

	postParams = NULL;
	httpHdrs = NULL;
}


TTask::TTask(string in_url, TASK_TYPE in_task_type)
{		
	useProxy = false;
	
	url = new URL();
	url->set_url(in_url.c_str());
	task_type = in_task_type;

	hcp = new THttpCli(in_url.c_str());
	init();	
}

TTask::TTask(string in_url, TASK_TYPE in_task_type, Proxy_t proxyInfo)
{	
	useProxy = true;

	url = new URL();
	url->set_url(in_url.c_str());
	task_type = in_task_type;	
	
	hcp = new THttpCli(in_url.c_str(), proxyInfo);
	init();

}

TTask::~TTask()
{	//�ڲ�������
	//�˴�Ҫ�������socket���ӹر�
	LOG_DEBUG("[TTask]delete TTask begin\n");

	hcp->Disconnect();	
	LOG_DEBUG("[TTask]Disconnect hcp\n");
	
	if(NULL != hcp)
	{	
		delete hcp;
		hcp = NULL;
		LOG_DEBUG("[TTask]free hcp hcp\n");
	}
	if(NULL != url)
	{
		delete url;
		LOG_DEBUG("[TTask]free url hcp\n");
		url = NULL;
	}
	if(NULL != requestBuffer)
	{
		free(requestBuffer);
		requestBuffer = NULL;
		LOG_DEBUG("[TTask]free requestBuffer hcp\n");
	}	

	delete postParams;
	postParams = NULL;
	delete httpHdrs;
	httpHdrs = NULL;
	respData = NULL;
	respDataContent.clear();
	//TODO:.....
}

int TTask::generateSendDataBuffer()
{
	int ret = 0;
	//��� ---

	//���֮ǰ������ŵİ�����ɾ��֮(�������ض���ʱ)
	if(NULL != requestBuffer)	
	{
		free(requestBuffer);
	}
	
	ULONG64 methodlen = 0;
	ULONG64	urllen = 0;
	
	if(hcp->IsUseProxy())
	{
		urllen = strlen(url->get_url());
	}
	else
	{
		urllen = strlen(url->get_encoded_path());
	}

	if(task_type == TASK_DOWNLOAD)
	{
		methodlen = HTTP_METHOD_GET_FIXLEN + urllen;
	}
	else if(task_type == TASK_HEAD)
	{
		methodlen = HTTP_METHOD_HEAD_FIXLEN + urllen;
	}
	else if(task_type == TASK_QUERY 
				|| task_type == TASK_UPLOAD 
				|| task_type == TASK_BINARY )
	{
		methodlen = HTTP_METHOD_POST_FIXLEN + urllen;
	}
	
	requestBufferSize = methodlen 
				  + hcp->GetHttpRequestLen() + strlen("\r\n") //��Ϊ��Ϣͷ+����
				  + postDataSize	//��ΪPost��������
				  + 1;				//��Ϊ������Ϣ��Ľ�������������socket����
				  
	requestBuffer =  (char*)malloc(sizeof(char) * (size_t)requestBufferSize);

	if(NULL == requestBuffer)
	{
		return RET_NO_MEMORY;
	}

	LOG_DEBUG("[generateSendDataBuffer]Before generateSendDataBuffer methodlen=%lld headerlen=%lld blankline=2 postDataSize=%lld buffersize=%lld\n",
					methodlen, hcp->GetHttpRequestLen(), postDataSize, requestBufferSize);
	ULONG64	realRequestBufferSize = requestBufferSize;
	if(TASK_DOWNLOAD == task_type)	//������������ʹ��Get ����
	{
		ret = hcp->BuildHttpRequest(VERB_GET, 
							  requestBuffer, requestBufferSize, 
							  NULL, task_type, requestSize);	
	}
	else if(task_type == TASK_HEAD)	//������������ʹ��Get ����
	{
		ret = hcp->BuildHttpRequest(VERB_HEAD, 
							  requestBuffer, requestBufferSize, 
							  NULL, task_type, requestSize);	
	}
	else
	{
		ret =hcp->BuildHttpRequest(VERB_POST, 
							  requestBuffer, requestBufferSize, 
							  postParams, task_type, requestSize);	
	}

	if(RET_OK != ret)
	{
		LOG_DEBUG("BuildHttpRequest FAIL ret=%d", ret);
		return ret;
	}

	if(realRequestBufferSize != requestBufferSize)
	{		
		LOG_DEBUG("Post content size changed oldSize=%lld newsize=%lld.",
				 requestBufferSize, realRequestBufferSize);
		requestBufferSize = realRequestBufferSize;
		return RET_FILE_CHANGED;
	}
	
	int nHdr;
	THttpHdr * pHdr;
	ShowMsg(hcp->GetStatusLine());
	nHdr = (int)hcp->GetHttpRequest()->GetHdrCount();
   	for(int i = 0; i < nHdr; i++)
	{
		pHdr = hcp->GetHttpRequest()->GetHdr( i );
		
		LOG_DEBUG("%s: %s", pHdr->GetKey().c_str(), pHdr->GetVal().c_str());
   	}

	//��ӡ��õİ� ---�������2�������ݣ����ӡ�����ж� 
	//ShowMsg(requestBuffer);
	return ret;
}



/******************************************************************************
 * TTask.sendHttpReqContinue - ����Ϊ�������http��������
 * DESCRIPTION: - 
 *    n/a 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 26nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
int TTask::sendHttpReqContinue()
{
	int ret = 0;
	ULONG64 BytesWritten;

	//������������
	ret = hcp->SendData(requestBuffer + requestSentSize, 
						requestSize - requestSentSize, 
						0, 100, BytesWritten);

	LOG_DEBUG("[sendHttpReqContinue]SendData ret=%d requestSize=%lld ask_send=%lld real_send=%lld\n", 
					ret, requestSize, requestSize-requestSentSize, BytesWritten);

	//���ݷ��������Ǩ����״̬
	switch(ret)
	{	 
		case RET_SOCKET_FINISH:	//�����Ѿ��ɹ����ͣ���ȴ������У���Ǩ��״̬			
		case RET_SOCKET_CONTINUE:
			requestSentSize += BytesWritten;
            last_active_time = time(NULL);
			break;
		case RET_SOCKET_ERROR:
		case RET_SOCKET_CLOSE:
		default:
			LOG_WARN("[sendHttpReqContinue]fail ret=%d old status=%d, set task to TSK_ERROR\n", 
						ret, taskState);	
			taskState = TSK_ERROR;
			last_active_time = time(NULL);
			errcode = ERR_TASK_SOCKET_SEND;
			break;			
	}
	LOG_INFO("sendHttpReqContinue url=%s last_active_time=%ld\n", url->get_url(), last_active_time);	
	return ret;
}


/******************************************************************************
 * getRemoteTargetInfo - ����׼������
 * DESCRIPTION: - 
 *    �ú�����Ҫ��Ŀ�Ķ˷��������Եõ����յ���ʵurl������ȡĿ���ļ���С�� 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 17nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
INT32 TTask::sendHttpReq()
{
	int ret = 0;
    ULONG64 BytesWritten = 0;

	//�����ظ���������
	shouldGetFileInfo = false;
	ret = setHttpHeader();
	if(RET_OK != ret)
	{	
		LOG_WARN("[sendHttpReq]setHttpHeader fail ret=%d old status=%d, set task to TSK_ERROR\n", 
					ret, taskState);	
		taskState = TSK_ERROR;		
		last_active_time = time(NULL);
		errcode = ret_to_err(ret);
		return ret;
	}

	ret = generateSendDataBuffer();
	if(RET_OK != ret)
	{ 
		LOG_WARN("[sendHttpReq]generateSendDataBuffer fail ret=%d old status=%d, set task to TSK_ERROR\n", 
					ret, taskState);	
		 
		taskState = TSK_ERROR;		
		last_active_time = time(NULL);
		errcode = ret_to_err(ret);
		return ret;
	}	
	
	//������������
	ret = hcp->SendData(requestBuffer, requestSize, 0, 100, BytesWritten);
	LOG_DEBUG("[sendHttpReq]SendData ret=%d ask_send=%lld real_send=%lld\n", 
							ret, requestSize, BytesWritten);

	//���ݷ��������Ǩ����״̬
	switch(ret)
	{	 
		case RET_SOCKET_FINISH:	//�����Ѿ��ɹ����ͣ���ȴ������У���Ǩ��״̬			
		case RET_SOCKET_CONTINUE:
			taskState = TSK_RUNNING;			
			last_active_time = time(NULL);
			requestSentSize = BytesWritten;			
			break;
		case RET_SOCKET_ERROR:
		case RET_SOCKET_CLOSE:
		default:
			LOG_WARN("[sendHttpReq]SendData fail ret=%d old status=%d, set task to TSK_ERROR\n", 
						ret, taskState);	
			taskState = TSK_ERROR;			
			last_active_time = time(NULL);
			errcode = ERR_TASK_SOCKET_SEND;
			break;			
	}

	return ret;
}

int TTask::processHttpResp()
{
	int ret = 0;

	if(!respHeadCompleted)
	{
		//��Ӧͷδ��ȡ����������Ӧͷ
		ret = processHttpRespHead();
	}
	else
	{
		//��Ӧͷ�Ѿ��õ�����ʼ��ȡ��Ӧ������
		ret = getRealData();
	}

	return ret;
}


/******************************************************************************
 * TTask.processHttpResqHead - �������󷵻��룬��������httpͷ
 * DESCRIPTION: - 
 *    n/a 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 18nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
INT32 TTask::processHttpRespHead()
{
	int ret = 0;
	char szBuffer[HTTP_BUF_SIZE];
	ret = hcp->ProcessHttpResponse(szBuffer);
	last_active_time = time(NULL);

	LOG_INFO("[ProcessHttpResponse]return ret=%d\n", ret);
	LOG_INFO("processHttpRespHead url=%s last_active_time=%ld\n", url->get_url(), last_active_time);	
	switch(ret)
	{
		case RET_OK:	//��ȡ�ɹ�����Ǩ��״̬, ��ȡget����
		{
			respHeadCompleted = true;
			//taskState = TSK_RUNNING;	//��Ǩ��״̬
			//����http��Ӧͷд��Ӧ���ݴ�С,�����contentlength,��Ϊ-1
			taskInfo.contentLength = hcp->GetContentLen();
			taskInfo.contentLength = taskInfo.contentLength == 0? -1:taskInfo.contentLength;
			if(task_type == TASK_HEAD)
			{
				//��Ӧͷ��ȡ�ɹ�,��head������˵�������Ѿ����
				taskState = TSK_FINISHED;
				respContentCompleted = true;
				return RET_SOCKET_FINISH;
			}
			break;
		}
		
		case RET_NEWDIRECTION:
		{
			//�˴���Ҫ��������url
			string newlocation;
			if(NULL != hcp->GetLocation(newlocation)
				&& 0 != newlocation.length())
			{				
				//�˴��������������ӣ���ɨ���������ʱ���ٴη�������
				hcp->Disconnect();
				url->reset_url(newlocation.c_str());
				hcp->ResetUrl(newlocation.c_str());
				//��״̬��ΪTSK_CONNECT���ȴ���һ������
				taskState = TSK_CONNECT;	
				last_active_time = time(NULL);
			}
			else
			{
				LOG_WARN("[processHttpRespHead]get new location old status=%d, set task to TSK_ERROR\n", 
							taskState);	
				taskState = TSK_ERROR;
				errcode = ERR_TASK_REDIRECTION_NOTARGET;				
				return RET_CLOSE;
			}
			//taskState = TSK_START;	//������Ǩ�ƻ�Start״̬���Ա����·�������
			shouldGetFileInfo = true;
			redirectionTimes++;
			return RET_NEWDIRECTION;
		}

		case RET_PROXY_AUTH_UNSUPPORTED:
		{
			if((hcp->IsUseProxy() && hcp->GetProxy().type == HTTPDOWN_PROXY_HTTP)
				&& hcp->GetProxy().domain != "")
			{
				//Using Ntlm proxy				
				int tmpret = hcp->processNtlmAuth();
				if(tmpret != RET_OK)
				{
					//����			
					LOG_WARN("[processHttpRespHead]ProcessHttpResponse when do processNtlmAuth ret=%d old status=%d, set task to TSK_ERROR\n", 
								tmpret, taskState);	
					taskState = TSK_ERROR;
					errcode = ERR_TASK_READ_RESQ_FAIL;				
					return RET_CLOSE;
				}
				else
				{
					LOG_INFO("[processHttpRespHead]ProcessHttpResponse do processNtlmAuth ret=%d old status=%d, set task to TSK_START\n", 
								ret, taskState);	
					taskState = TSK_START;	//������Ǩ�ƻ�Start״̬���Ա����·�������
					shouldGetFileInfo = true;
					hcp->ClearResponseHdr();
					return RET_PROXY_DOING_AUTH;
				}
			}
			else
			{
				//����   
				LOG_WARN("[processHttpRespHead]ProcessHttpResponse fail ret=%d old status=%d, proxy fail for:type:%d, domain:%s, isProxy:%d, set task to TSK_ERROR\n", 
					ret, taskState, hcp->GetProxy().type, hcp->GetProxy().domain.c_str(), hcp->IsUseProxy()); 
				taskState = TSK_ERROR;
				errcode = ERR_TASK_READ_RESQ_FAIL;    
				return RET_CLOSE;    
			}
		}
		
		case RET_SOCKET_CONTINUE:
		{
			return RET_SOCKET_CONTINUE;
		}
		
		case RET_PARSE_RESQ_FAIL:
		{
			LOG_WARN("[processHttpRespHead]ProcessHttpResponse fail ret=%d old status=%d, set task to TSK_ERROR\n", 
						ret, taskState);	
			taskState = TSK_ERROR;
			errcode = ERR_TASK_PARSE_RESQ_FAIL;				
			return RET_CLOSE;
		}
		case RET_SOCKET_CLOSE:			
		case RET_SOCKET_ERROR:
		default:
		{
			//����			
			LOG_WARN("[processHttpRespHead]ProcessHttpResponse fail ret=%d old status=%d, set task to TSK_ERROR\n", 
						ret, taskState);	
			taskState = TSK_ERROR;
			errcode = ERR_TASK_READ_RESQ_FAIL;				
			return RET_CLOSE;
		}
	}

	return RET_OK;
}


int TTask::setHttpHeader()
{
	hcp->ClearRequestHdr();
	//��д���ϲ㴫���httpͷ�������ͷ���ڲ�ͷ�ظ�������ں��������б�����
	if(NULL != httpHdrs)
	{
		THttpHdrList * httpHdrlist = httpHdrs->GetHdrs();
		
		THttpHdrList::iterator itr = httpHdrlist->begin();
		for( ; itr != httpHdrlist->end(); ++itr )
		{
			THttpHdr* pHdr = *itr;
			hcp->BuildCustomHdr(pHdr->GetKey().c_str(), pHdr->GetVal().c_str());
		}		
	}

	hcp->BuildAcceptHdr();

	if(TASK_UPLOAD == task_type || TASK_QUERY == task_type)
	{
//		hcp->BuildCustomHdr("Referer", url->get_encoded_path());
//		hcp->BuildCustomHdr("Accept-Language", "zh-cn");
		//Post ������Ҫд��post���ݴ�С
//			char contenttype[128] = {0};
//			snprintf(contenttype, sizeof(contenttype), 
//				"multipart/form-data; boundary=%s", __HEAD_BROUNDARY);
//			hcp->BuildContentType(contenttype);
		hcp->BuildContentType();

//		hcp->BuildCustomHdr("Accept-Encoding", "gzip, deflate");
		hcp->BuildUserAgentHdr();
		hcp->BuildHostHdr(url->get_host(), url->get_port());
		hcp->BuildContentLengthHdr(postDataSize);
		
	}
	else if(TASK_BINARY == task_type)
	{
		hcp->BuildUserAgentHdr();
		hcp->BuildHostHdr(url->get_host(), url->get_port());
		hcp->BuildContentLengthHdr(postDataSize);
	}
	else if(TASK_DOWNLOAD == task_type)
	{		
		hcp->BuildUserAgentHdr();
		hcp->BuildHostHdr(url->get_host(), url->get_port());
		if((startSize != 0) || (endSize != 0))
		{
			if(endSize == 0)
			{
				hcp->BuildRangeHdr(off_t(startSize), -1);
			}
			else
			{
				hcp->BuildRangeHdr(off_t(startSize), off_t(endSize));
			}
		}
	}
	else if(TASK_HEAD == task_type)
	{
		hcp->BuildUserAgentHdr();
		hcp->BuildHostHdr(url->get_host(), url->get_port());
	}
	
	
	if(hcp->IsUseProxy() && hcp->GetProxy().type == HTTPDOWN_PROXY_HTTP)
	{
		if(hcp->GetProxy().domain != "")
		{
			//It's ntlm proxy
			hcp->processNtlmAuth();
			hcp->BuildNtlmAuthHdr(hcp->getNtlmNegotiateStr().c_str());
			hcp->BuildCustomHdr("Proxy-Connection", "Close");
//				hcp->BuildCustomHdr("Connection", "Keep-Alive");
			hcp->BuildConnectionHdr();
		}
		else
		{
			hcp->BuildBasicAuthHdr(url->get_user(), url->get_password() );
			if(!hcp->GetProxy().user.empty())
			{
				hcp->BuildProxyAuthHdr(hcp->GetProxy().user.c_str(), hcp->GetProxy().pwd.c_str());
			}			
			hcp->BuildConnectionHdr();
		}
	}
	else
	{
	//hcp->BuildPragmaHdr();
		hcp->BuildConnectionHdr();
	}
	//hcp->BuildCustomHdr("Cache-Control", "no-cache");
	return RET_OK;
}

void TTask::updateTaskInfo()
{
	ULONG64 cur_time = (int)time(NULL);
	ULONG64 last_time = cur_time - taskInfo.startTime;
	ULONG64 currentSize = 0;

	dataChanged = false;	

	if(NULL != hcp)
	{
		taskInfo.httpCode = hcp->getStatusCode();
	}
	taskInfo.errcode = errcode;
	taskInfo.endTime = cur_time;
	taskInfo.respData = respData;

	if(hcp)
	{
		taskInfo.htrequest = hcp->GetHttpRequest();
		taskInfo.htresponse = hcp->GetHttpRequest();
	}
	else
	{
		taskInfo.htrequest = NULL;
		taskInfo.htresponse = NULL;
	}
	
	if(0 >= last_time)
	{
		last_time = 1;
		taskInfo.transferSpeed = 0;
		taskInfo.remainTime = -1;
//		taskInfo.doneSize = 0;
	}
	if(taskState == TSK_ERROR)
	{
		return;
	}
	
	switch(task_type)
	{		
		case TASK_DOWNLOAD:
		case TASK_QUERY:
		{
			if(dataClean)	//ÿ�ζ�ȡ��Ҫɾ����������
			{
				if(respData->size())
				{
					dataChanged = true;
				}
				taskInfo.doneSize += (int)respData->size();	
			}
			else
			{
				currentSize = taskInfo.doneSize;
				taskInfo.doneSize = (int)respData->size();
				if(currentSize != taskInfo.doneSize)
				{
					dataChanged = true;
				}
			}
			

			taskInfo.transferSpeed = taskInfo.doneSize/last_time;
			if(taskInfo.contentLength > 0 && taskInfo.transferSpeed > 0)
			{
				taskInfo.remainTime = (taskInfo.contentLength-taskInfo.doneSize)/taskInfo.transferSpeed;
			}
			else
			{
				taskInfo.remainTime = -1;
			}

			break;
		}
		
		case TASK_HEAD:
		{
			currentSize = taskInfo.doneSize;
			taskInfo.doneSize = (int)respData->size();
			if(currentSize != taskInfo.doneSize)
			{
				dataChanged = true;
			}

			taskInfo.remainTime = -1;
			break;
		}
		
		case TASK_UPLOAD:			
		case TASK_BINARY:
		{
			currentSize = taskInfo.doneSize;
			taskInfo.doneSize = requestSentSize;
			if(currentSize != taskInfo.doneSize)
			{
				dataChanged = true;
			}


			taskInfo.transferSpeed = taskInfo.doneSize/last_time;
			if(requestSize > 0 && taskInfo.transferSpeed > 0)
			{
				taskInfo.remainTime = (requestSize - taskInfo.doneSize)/taskInfo.transferSpeed;
			}
			else
			{
				taskInfo.remainTime = -1;
			}

			break;
		}
		
		default:
		{
			currentSize = taskInfo.doneSize;
			taskInfo.doneSize = (int)respData->size();
			if(currentSize != taskInfo.doneSize)
			{
				dataChanged = true;
			}


			taskInfo.transferSpeed = taskInfo.doneSize/last_time;
			if(taskInfo.contentLength > 0 && taskInfo.transferSpeed > 0)
			{
				taskInfo.remainTime = (taskInfo.contentLength-taskInfo.doneSize)/taskInfo.transferSpeed;
			}
			else
			{
				taskInfo.remainTime = -1;
			}
			break; 
		}
	}		
	
}
INT32 TTask::getRealData()
{
	int ret = 0;
	ULONG64 needSize = 0;
	if(taskInfo.contentLength > 0 
		&& task_type == TASK_DOWNLOAD
//			&& dataClean
		&& taskInfo.contentLength > (LONG64)taskInfo.doneSize)
	{
		needSize = taskInfo.contentLength - taskInfo.doneSize;
	}
	else
	{
		needSize = 0;
	}
	
	ret = hcp->DirectRecvData(respData, 
			needSize, 0);

	last_active_time = time(NULL);

	LOG_INFO("url=%s last_active_time=%ld\n", url->get_url(), last_active_time);	


	switch(ret)
	{
		case RET_SOCKET_ERROR:
			//����socket����Ǩ������״̬��ʧ��
			LOG_WARN("[getRealData]DirectRecvData fail ret=%d old status=%d, set task to TSK_ERROR\n", 
						ret, taskState);	
			taskState = TSK_ERROR;
			errcode = ERR_TASK_SOCKET_RECV;
			
			break;

		case RET_SOCKET_FINISH:
			//���ӱ��Զ˹رգ�
			//������Ӧ���ݳ�����֪�����ѽ��յ���Ӧ���ȣ���ӦǨ��״̬���ɹ�, ͬʱ�رն�ȡ��
			taskState = TSK_FINISHED;	
			respContentCompleted = true;
			LOG_INFO("[getRealData]DirectRecvData success ret=%d, set task to finish.\n", 
					  		ret);
			break;

		case RET_SOCKET_CONTINUE:
			//���ݲ��������ȴ��´����ݵ���
			break;

		default:
			break;
	}

	//TODO: ���ݽ����������TaskInfo
	
	return ret;
}

INT32 TTask::tryAuth()
{
	int ret = 0;
	
	ret = hcp->proxyAuth();
	last_active_time = time(NULL);
	if(ret == RET_OK)
	{
		taskState = TSK_START;	//������֤����
	}
	else if(RET_PROXY_DOING_AUTH != ret && RET_PROXY_BEGIN_AUTH != ret )
	{
		LOG_WARN("[tryAuth]proxyAuth fail ret=%d old status=%d, set task to TSK_ERROR\n", 
					ret, taskState);	
		taskState = TSK_ERROR;
		errcode = ERR_TASK_CONNECT_FAIL;
	}
		
	return ret;
}

INT32 TTask::tryConnect()
{	
	int ret = 0;
	taskInfo.startTime = (int)time(NULL);
	last_active_time = time(NULL);
	
	if(connectTimes >= MAX_RETRY_CONNECT)
	{	//���������ﵽ���ƣ�����ʧ��
		LOG_WARN("[tryConnect]MAX_RETRY_CONNECT reached old status=%d, set task to TSK_ERROR\n", 
					taskState);	
		taskState = TSK_ERROR;
		errcode = ERR_TASK_CONNECT_FAIL;
		return RET_FAIL;
	}

	if(useProxy)
	{
		ret = hcp->ConnectProxyHost();
	}
	else
	{
		ret = hcp->Connect();
	}
	connectTimes++;

	switch(ret)
	{
		case RET_SOCKET_ERROR:
		case RET_SOCKET_CREATE_FAIL:
			//����socket���󣬲��������ȴ���һ������;
			LOG_WARN("[tryConnect]fail ret=%d.\n", ret);
			break;

		case RET_OK:
			onConnected();
			break;

		case RET_SOCKET_CONTINUE:
			//������socket���ӵȴ�������
			taskState = TSK_CONNECTWAIT;
			LOG_INFO("[tryConnect]ret=%d, set task to TSK_CONNECTWAIT.\n", ret);
			break;

		default:
			break;
	}

	return ret;	
}

int TTask::onAuthResp()
{
	int ret = 0;
	ret = hcp->recvAuthResp();
	last_active_time = time(NULL);

	switch(ret)
	{
		case RET_OK:	//�������ɹ������Կ�ʼ��������		
			taskState = TSK_START;
			break;
		case RET_PROXY_DOING_AUTH:	//�������������ȴ���������
			break;
			
		default:		//���������Ϊ������ʧ��
			LOG_WARN("[onAuthResp]recvAuthResp fail ret=%d old status=%d, set task to TSK_ERROR\n", 
						ret, taskState);	
			taskState = TSK_ERROR;
			errcode = ERR_TASK_PROXY_AUTH_FAIL;
			break;
	}

	return ret;
}

int TTask::onConnected()
{
	int ret = 0;
	ret = hcp->checkConnect();
	last_active_time = time(NULL);
	if(RET_OK == ret)	//���ӳɹ�����socket���ã������������ȴ�����
	{
		if(useProxy)
		{	
			//���ӳɹ�����ʹ���˴�������һ����Ҫ���д�����֤
			taskState = TSK_AUTH;
			LOG_INFO("[onConnected]OK, set task to TSK_AUTH.\n", ret);
		}
		else	//û��ʹ�ô�����Ǩ�Ƶ�TSK_START��ʼ������
		{
			taskState = TSK_START;
			LOG_INFO("[onConnected]OK, set task to TSK_START.\n", ret);
		}
	}
	else
	{
		LOG_WARN("[onConnected]FAIL, ret=%d.\n", ret);

	}
	return RET_OK;
}

INT32 TTask::checkConnectTimeout()
{
	int ret = 0;
	ULONG64 currenttime = (int)time(NULL);	

	if(currenttime - taskInfo.startTime > MAX_CONNECT_TIMEOUT)
	{
		hcp->Disconnect();			//�Ͽ���ǰ������
		taskState = TSK_CONNECT;	//����������Ϊ׼����ʼ����
		
		LOG_WARN("[checkConnectTimeout]task timeout currenttime=%ld starttime=%ld MAX_CONNECT_TIMEOUT=%d, set task to TSK_CONNECT.\n", 
						currenttime, taskInfo.startTime, MAX_CONNECT_TIMEOUT);
		return RET_CONNECT_TIMEOUT;
	}

	return RET_OK;
}


INT32 TTask::dumpDataToFile()
{
	FILE* fp = NULL;
	if(NULL == url->get_file() || 0 == strcmp("", url->get_file()))
	{
		fp = fopen("default.unknow", "w+b");
	}
	else
	{
		fp = fopen(url->get_file(), "w+b");
	}
	
	if(NULL == fp)
	{
		LOG_DEBUG("[dumpDataToFile]Open file=%s fail\n", 
			url->get_file());
		return -1;
	}
	else
	{
		for(UINT i = 0; i < respData->size(); i++)
		{
			char ch = (*respData)[i];
#if 0
			if((*respData)[i] == 'q')
			{
				for(int j=0; j<20; j++)
				{
					LOG_DEBUG("ch=%d ", (*respData)[i+j]);
				}
				LOG_DEBUG("\n");
			}
#endif
			fwrite(&ch, sizeof(char), 1,  fp);
		}
		fclose(fp);
		return 0;
	}
}

void TTask::checkTimeOut()
{
	if(timeoutlimit > 0)
	{
		ULONG64 cur_time = (int)time(NULL);
		ULONG64 last_time = cur_time - last_active_time;

		if(0 >= last_time)
		{
			taskInfo.transferSpeed = 0;
			taskInfo.remainTime = -1;
		}

		if(last_time >= timeoutlimit)
		{
			LOG_WARN("[checkTimeOut]Task timeout timeoutlimit=%d, cur_time=%d set task to TSK_ERROR\n", 
						timeoutlimit, cur_time);	

			LOG_WARN("[checkTimeOut]Task timeout last_active_time=%d old status=%d, set task to TSK_ERROR\n", 
				last_active_time, (int)taskState);	

			taskState = TSK_ERROR;
			errcode = ERR_TASK_TIMEOUT;
		}
	}
}


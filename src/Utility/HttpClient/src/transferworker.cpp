#include "stdafx.h"


#ifndef	 HTTPTRANSFER_RELEASE
#include "log.h"
#else
#include "log_stun.h"
#endif
#include "transferworker.h"
#include "HttpCli.h"



//#include "task.h"

#define WORKER_SLEEP_TIME 10				//Unit -- ms


INT32 TTransferWorker::findTaskPos()
{
	int i = curPos;	
	do
	{
		if(NULL == taskQue[i])
		{	//�ҵ�����λ��, ��curPos����Ϊ��ǰλ�õ���һ��
			curPos = (i+1)%MAX_TASKQUE_SIZE;

			if(i > maxPos)
			{
				maxPos = i;
			}

			return i;
		}
		
		i++;
		if(i == MAX_TASKQUE_SIZE)
		{	//��������ĩ�ˣ���ͷ��ʼѰ��
			i = 0;
		}
	}while(i != curPos);	//��i == curPos, �����ȫ�����в�����ϣ���δ�ҵ�����λ��


	return -1;	//����ʧ�ܣ��޿���λ��
}

void TTransferWorker::setMaxRunTaskNum(UINT32 in_maxRunTaskNum)
{
	maxRunTaskNum = in_maxRunTaskNum;
}


UINT32 TTransferWorker::getRunningTaskCount()
{
	return runningTaskCount;
}


/******************************************************************************
 * TTransferWorker.scanTaskQue - ɨ����������
 * DESCRIPTION: - 
 *    ��������ǰ״ִ̬����Ӧ����������״̬��Ǩ 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 18nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
void TTransferWorker::scanTaskQue()
{
	int i = 0;
	int ret = 0;
	TTask* pTask = NULL;

	int runningCount = 0;
	int startingCount = 0;
	int waittingCount = 0;
	int connectingCount = 0;
	int finishCount = 0;
	int errorCount = 0;
	int unkownCount = 0;
	
	LOG_DEBUG("Scan taskQue begin, tasksize=%d runningCount=%d startingCount=%d waittingCount=%d finishCount=%d errorCount=%d unkownCount=%d\n",
		MAX_TASKQUE_SIZE, runningTaskCount, startingTaskCount, waittingTaskCount, finishTaskCount, errorTaskCount, unkownTaskCount);
	
	int scheduledcount = runningTaskCount
							+startingTaskCount
							+finishTaskCount
							+errorTaskCount
							+unkownTaskCount
							+connectingTaskCount;

	int	remainposition = 0;
	if((int)maxRunTaskNum > scheduledcount)
	{
		remainposition = maxRunTaskNum - scheduledcount;
	}


	//ɨ����������
	for(i = 0; i <= maxPos; i++)
	{
		if(stop)
		{
			LOG_INFO("[scanTaskQue]Found worker stop set, break task quene process\n");		
			return;
		}


		if(NULL == taskQue[i])
		{					
			continue;	//��λ��û������
		}

		pTask = taskQue[i];
		if(pTask->taskState != TSK_IDLE)
		{
			pTask->checkTimeOut();
		}		

		//�˴����������ں�httpmanager��canceltask, clearͬ�����Ӷ���֤����
		//����canceltask, clear���غ�����������Ļص������ֱ�ִ�е������
		//���������������ܳ���pTask->del���ͨ����֮��canceltask�����ã���
		//�ܿ췵�أ�Ȼ�󱾺�����task״̬�������ֵ���task�Ļص����������
//			thread_lock(&task_que_lock);	
		if(pTask->del)
		{
			clearTaskInfo(i);			
			LOG_INFO("Task=%d was cancelled.\n", i);
//				thread_unlock(&task_que_lock);
			continue;
		}

		if(pTask->respData == NULL)
		{
			//���������ڲ���������ʧ��---ԭ��δ֪���˴����б���
			pTask->taskState = TSK_ERROR;
			pTask->errcode = ERR_TASK_PARAM_RESPDATA;
			LOG_WARN("[scanTaskQue]respData is NULL. old status=%d, set task to TSK_ERROR\n", 
						ret, pTask->taskState);	
		}		
		
		switch(pTask->taskState)
		{
			case 	TSK_IDLE:
			{
				if(remainposition > 0
					&& s_t_map.size()<64)
				{			
					//��ǰ����������δ�ﵽ����
					pTask->taskState = TSK_CONNECT;
					remainposition --;
				}
				else
				{				
					LOG_DEBUG("MxRunTaskNum=%d currentRun=%d, mapsize=%d, limit reached.\n", 
						maxRunTaskNum, getRunningTaskCount(), s_t_map.size());
				}
				waittingCount++;
				break;					
			}

			case	TSK_CONNECT:
			{				
				//��ǰ����������δ�ﵽ����ֵ�����Լ���
				ret = pTask->tryConnect();
				LOG_INFO("tryActiveTask ret=%d for task=%d\n\n", ret, i);

				if(RET_SOCKET_CONTINUE == ret)
				{
					//�첽connect,�ȴ�socket ����,��¼socket��task�Ķ�Ӧ��ϵ
					s_t_map[pTask->hcp->getSocket()->get_socketfd()] = i;			
					FD_SET(pTask->hcp->getSocket()->get_socketfd(), 
									&writefds);
					setMaxfd(pTask->hcp->getSocket()->get_socketfd());					
				}

				if(RET_OK == ret)	//����ֱ�ӳɹ�����¼socket��task�Ķ�Ӧ��ϵ
				{
					s_t_map[pTask->hcp->getSocket()->get_socketfd()] = i;
				}
				
				break;
			}

			case   TSK_CONNECTWAIT:
			{	
				//�ȴ����ӳɹ��У���Ҫ����Ƿ�ʱ
				int sockfd = pTask->hcp->getSocket()->get_socketfd();
				ret = pTask->checkConnectTimeout();
				LOG_DEBUG("checkConnectTimeout ret=%d for task=%d\n\n", ret, i);
				if(RET_CONNECT_TIMEOUT == ret)
				{					
					FD_CLR(sockfd, &writefds);
					s_t_map.erase(sockfd);
				}
				
				connectingCount++;
				break;
			}

			case TSK_AUTH:
			{
				ret = pTask->tryAuth();
				if(ret == RET_PROXY_BEGIN_AUTH)
				{
					//��ʼ����socks��֤����Ҫ��ȡ��֤��Ӧ
					LOG_INFO("Socks authing for task=%d, set fd=%d, wait resq.\n", 
									i, pTask->hcp->getSocket()->get_socketfd());
					FD_SET(pTask->hcp->getSocket()->get_socketfd(), 
							&readfds);
					setMaxfd(pTask->hcp->getSocket()->get_socketfd());						
				}
				break;
			}
			
			case 	TSK_START:				
			{	//����Ƿ��ȡ����ʵ��ַ������ɹ��������󣬿�ʼ���أ�״̬Ǩ����running
				if(MAX_REDIRECTIONS <= pTask->redirectionTimes)
				{	//�ض��������������
					LOG_WARN("MAX_REDIRECTIONS=%d redirectionTimes=%d, limit reached.\n", 
								MAX_REDIRECTIONS, pTask->redirectionTimes);
					pTask->taskState = TSK_ERROR;
					pTask->errcode = ERR_TASK_REDIRECTION_EXCEED;
				}
				else
				{	
					//�ж��Ƿ�Ҫ���·������󣬴˱�־��һ�η���������Ϊtrue
					//�����������Ϊfalse���յ��ض���������������Ϊtrue
					if(pTask->shouldGetFileInfo)
					{
						LOG_INFO("Send Req for task=%d.\n", i);
						ret = pTask->sendHttpReq();
								
						if(RET_SOCKET_FINISH == ret)
						{	
							//����http����ɹ������ö�ȡ��ǣ��ȴ���Ӧ����
							LOG_INFO("Send Req success for task=%d, set fd=%d, wait resq.\n", 
									i, pTask->hcp->getSocket()->get_socketfd());
							FD_SET(pTask->hcp->getSocket()->get_socketfd(), 
									&readfds);
							setMaxfd(pTask->hcp->getSocket()->get_socketfd());						
						}
						else if(RET_SOCKET_CONTINUE == ret)
						{	
							//������������ϴ�����д���ǣ��ȴ�socket��д�����д��
							LOG_DEBUG("Send Req block for task=%d, set fd=%d, wait resq.\n", 
											i, pTask->hcp->getSocket()->get_socketfd());
							FD_SET(pTask->hcp->getSocket()->get_socketfd(), 
									&writefds);
							setMaxfd(pTask->hcp->getSocket()->get_socketfd());							
						}
						else						
						{
							//����HTTP����ʧ�ܣ�����ʧ��
							LOG_WARN("Send Req fail ret=%d for task=%d.\n", ret, i);
						}
					}
					else
					{
						pTask->updateTaskInfo();
						
						if(NULL != pTask->handle_init)
						{
							thread_lock(&task_que_lock);	
							if(!pTask->del)
							{
								pTask->handle_init(i, &pTask->taskInfo);
							}
							thread_unlock(&task_que_lock);	
						}
						LOG_INFO("Req already sent for task=%d, waiting resq.\n", i);
					}
					
				}
				startingCount++;
				break;
			}
			
			case 	TSK_RUNNING:
			{				
				LOG_DEBUG("task=%d running, try call callback.\n", i);
				//�˴���������ص������㱨������Ϣ
				pTask->updateTaskInfo();
				if(NULL != pTask->handle_process
						&& pTask->dataChanged)
				{
					thread_lock(&task_que_lock);	
					if(!pTask->del)
					{
						pTask->handle_process(i, &pTask->taskInfo);
					}
					thread_unlock(&task_que_lock);	
					if(pTask->dataClean)
					{
						LOG_DEBUG("task=%d data changed and dataClean set, Clean current data --len=%d.\n", 
									i, pTask->respData->size());
						pTask->respData->clear();
					}
				}
				else
				{
					LOG_DEBUG("task=%d data not change, Don't call callback.\n", i);
				}
				runningCount++;
				break;
			}
				
			case 	TSK_FINISHED:
			{
				LOG_INFO("task=%d finished, try call callback, and delete task info.\n", i);
				//�˴���������ص������㱨������Ϣ
				pTask->updateTaskInfo();
				if(NULL != pTask->handle_finish)
				{
					thread_lock(&task_que_lock);	
					if(!pTask->del)
					{
						pTask->handle_finish(i, &pTask->taskInfo);
					}
					thread_unlock(&task_que_lock);	
				}	
				

				//TODO:�˴�Ӧ������������Ϣ
				clearTaskInfo(i);
				
				finishCount++;
				break;
			}
			
			case 	TSK_ERROR:
			{
				LOG_WARN("task=%d Error, try call callback, and delete task info.\n", i);
				//�˴���������ص������ϱ�����
				pTask->updateTaskInfo();
				if(NULL != pTask->handle_error)
				{
					thread_lock(&task_que_lock);	
					if(!pTask->del)
					{
						pTask->handle_error(i, &pTask->taskInfo);
					}
					thread_unlock(&task_que_lock);	
				}	

				//TODO:�˴�Ӧ������������Ϣ
				clearTaskInfo(i);

				errorCount++;
				break;
			}
			
			default:
				LOG_WARN("task=%d Status Unkonw, delete task info.\n", i);
				//TODO:�˴�Ӧ������������Ϣ
				pTask->errcode = ERR_TASK_UNKNOWN_STATUS;	
				pTask->updateTaskInfo();
				if(NULL != pTask->handle_error)
				{
					thread_lock(&task_que_lock);	
					if(!pTask->del)
					{
						pTask->handle_error(i, &pTask->taskInfo);
					}
					thread_unlock(&task_que_lock);	
				}
				clearTaskInfo(i);
				unkownCount++;
				break;			
		}		
//			thread_unlock(&task_que_lock);
	}


	runningTaskCount =	 runningCount;	
	startingTaskCount =  startingCount; 
	waittingTaskCount =  waittingCount; 
	finishTaskCount =	 finishCount;	
	errorTaskCount =	 errorCount;	
	unkownTaskCount =	 unkownCount;	
	connectingTaskCount = connectingCount;
	
	LOG_DEBUG("Scan taskQue finished, tasksize=%d runningCount=%d startingCount=%d waittingCount=%d finishCount=%d errorCount=%d unkownCount=%d\n",
		MAX_TASKQUE_SIZE, runningTaskCount, startingTaskCount, waittingTaskCount, finishTaskCount, errorTaskCount, unkownTaskCount);

}

int TTransferWorker::scanIO()
{
	//��ʼselect IO ����
	
	int nreadys = 0;
	int readed = 0;
	int wrote = 0;
	timeval tm;	
	fd_set tmpreadfds;
	fd_set tmpwritefds;
	int ret = 0;
	TTask* pTask = NULL;

	Socket_task_map	delete_map;
	Socket_task_map	insert_map;
	
	//����Ϊ�˼ӿ�ѭ����������500us����ʱ
	tm.tv_sec = 0;
	tm.tv_usec = 500;
	
	LOG_DEBUG("[scanIO]Set %d read sockets %d write sockets, mapsize %d, maxfd %d\n", 
						readfds.fd_count, writefds.fd_count, s_t_map.size(), maxfd);
	
	if(0 == readfds.fd_count && 0 == writefds.fd_count)
	{
		if(s_t_map.size() != 0)
		{
			LOG_WARN("[scanIO]Set %d read sockets %d write sockets, mapsize %d\n", 
				readfds.fd_count, writefds.fd_count, s_t_map.size());
		}
		else
		{
			//LOG_DEBUG("[scanIO]No read or write sockets select.\n");
			return RET_NO_SOCKET_READY;
		}
	}
	
	tmpreadfds = readfds;
	tmpwritefds = writefds;
	nreadys = select(maxfd+1, &tmpreadfds, &tmpwritefds, NULL, &tm);
	if (nreadys > 0)
	{
		Socket_task_map::iterator it = s_t_map.begin();
		int sockfd = 0;
		int taskid = 0;
		delete_map.clear();
		insert_map.clear();
		for(; it != s_t_map.end(); it++)
		{	//Find the matched socket fd.
			if(stop)
			{
				LOG_INFO("[scanIO]Found worker stop set, break s_t_map IO process\n");
				
				return RET_WORKER_STOPED;
			}

			sockfd = it->first;
			taskid = it->second;
			pTask =  taskQue[taskid];

			if(TSK_CONNECTWAIT > pTask->taskState 
				|| TSK_RUNNING < pTask->taskState) 
			{
				//���м�״̬�յ�����, ��������
				continue;
			}

			if(pTask->respData == NULL)
			{
				//���������ڲ���������ʧ��---ԭ��δ֪���˴����б���
				pTask->taskState = TSK_ERROR;
				pTask->errcode = ERR_TASK_PARAM_RESPDATA;
				LOG_WARN("[scanIO]respData is NULL. old status=%d, set task to TSK_ERROR\n", 
							ret, pTask->taskState);	
			}	
			
			/******************************************************/
			//�������ݣ�����http����һ�㷢������Post�ϴ��ļ���������������ʱ//
			/******************************************************/
			if(FD_ISSET(sockfd, &tmpwritefds))
			{
				LOG_DEBUG("Socket=%d of task=%d state=%d ready for write.\n", 
					sockfd, it->second, pTask->taskState);

				//����TSK_CONNECTWAIT����ɶ�����������ӳɹ� ---�˴����⴦��
				if(TSK_CONNECTWAIT == pTask->taskState)
				{
					pTask->onConnected();
					FD_CLR(sockfd, &writefds);		//������					
					continue;
				}

				ret = pTask->sendHttpReqContinue();
				if(RET_SOCKET_FINISH == ret)
				{	
					//����http����ɹ���ɾ��д��ǣ����ö�ȡ��ǣ��ȴ���Ӧ����
					LOG_INFO("Send Req success for task=%d, set fd=%d, wait resq.\n", 
							taskid, pTask->hcp->getSocket()->get_socketfd());
					FD_SET(pTask->hcp->getSocket()->get_socketfd(), 
							&readfds);
					FD_CLR(sockfd, &writefds);											
				}
				else if(RET_SOCKET_CONTINUE == ret)
				{	
					//������������ϴ󣬵ȴ�socket����һ�ο�д�����д��
					LOG_DEBUG("Send Req block for task=%d, set fd=%d, wait resq.\n", 
							taskid, pTask->hcp->getSocket()->get_socketfd());							
				}
				else						
				{
					//����HTTP����ʧ�ܣ�����ʧ��  --- �˴�Ӧ���д���
					FD_CLR(sockfd, &writefds);	
					//��Ҫɾ���ϵ�socket��task�Ķ�Ӧ��ϵ
					delete_map[sockfd] = taskid;
					LOG_WARN("Send Req fail ret=%d for task=%d.\n", ret, taskid);
				}	
				LOG_DEBUG("write data to socket=%d of task=%d success\n", sockfd, taskid);
				wrote++;
			}


			/******************************************************/
			//��������_������Ӧͷ����Ӧ��//
			/******************************************************/
			if(FD_ISSET(sockfd, &tmpreadfds))
			{	//Found target fd.			
				LOG_DEBUG("Socket=%d of task=%d state=%d ready for read.\n", 
					sockfd, it->second, pTask->taskState);

				//����TSK_AUTH����ɶ����������socks��֤�� ---�˴����⴦��
				if(TSK_AUTH == pTask->taskState)
				{
					ret = pTask->onAuthResp();
					
					//���ڴ������У���ȴ���һ�����ݵ�������������������
					if(ret != RET_PROXY_DOING_AUTH)
					{
						FD_CLR(sockfd, &readfds);		//������	
					}
					continue;
				}
	
				ret = pTask->processHttpResp();
				if(ret != RET_OK && ret != RET_SOCKET_CONTINUE)
				{	//�������Ѳ���Ҫ��������, �������������ӳ��
					if(ret == RET_PROXY_DOING_AUTH)
					{
						LOG_INFO("processHttpResp ret=%d, socket=%d of task=%d\n", 
									ret, sockfd, taskid);
					}
					else
					{
						LOG_INFO("processHttpResp success ret=%d, Release socket=%d of task=%d\n", 
									ret, sockfd, taskid);
						//��Ҫɾ���ϵ�socket��task�Ķ�Ӧ��ϵ
						delete_map[sockfd] = taskid;	
					}
					FD_CLR(sockfd, &readfds);	//���ٴ������ȡ����
					
				}
				else
				{
					//������Ҫ��canceltask,clear��ͬ����������pTask->del���ĵط���Ҫ����		
					thread_lock(&task_que_lock);	
					if(	!pTask->del					
						&& pTask->respHeadCompleted 
						&& pTask->handle_headcomplete)
					{
						//��Ӧͷ�Ѿ��õ������ûص������������Ѿ��յ���Ӧͷ					
						pTask->updateTaskInfo();
						LOG_INFO("CALL handle_headcomplete FOR task=%d\n", taskid);
						pTask->handle_headcomplete(taskid, &pTask->taskInfo);
						//�ûص�����ֻ����һ��
						pTask->handle_headcomplete = NULL;
					}
					thread_unlock(&task_que_lock);
					LOG_DEBUG("read data from socket=%d of task=%d success\n", sockfd, taskid);
				}
				
				readed++;
			}
			else	//��socketû�����ݵ���
			{
				LOG_DEBUG("NO data from socket=%d of task=%d\n", sockfd, taskid);
			}
		}
		LOG_DEBUG("All s_t_map scaned, processed readed=%d wrote=%d of nreadys=%d\n", readed, wrote, nreadys);

		for(it = delete_map.begin(); it != delete_map.end(); it++)
		{
			LOG_DEBUG("Delete sock=%d of task=%d\n", 
						it->first, it->second);
			s_t_map.erase(it->first);
		}

		for(it = insert_map.begin(); it != insert_map.end(); it++)
		{
			LOG_DEBUG("Insert sock=%d of task=%d\n", 
						it->first, it->second);
			s_t_map[it->first] = it->second; 
		}
		
	}
	else if(0 == nreadys)
	{
		LOG_DEBUG("No socket ready\n");
		return RET_NO_SOCKET_READY;
	}
	else
	{
		LOG_DEBUG("Select failed ready = %d errno=%d errinfo=%s\n", 
				nreadys, socket_errno(), socket_errinfo());
		return RET_SOCKET_SELECT_FAIL;
	}	
	return RET_OK;
}


void TTransferWorker::setMaxfd(int newfd)
{
	maxfd = maxfd > newfd? maxfd:newfd;
}


INT32 TTransferWorker::run()
{
	int i = 0;
	int ret = 0;
	
	while(!stop)
	{
		LOG_DEBUG("Begin worker intevel %d\n", i);
		//ɨ��������У����״̬����Ǩ�Ƶȹ���
		scanTaskQue();

		//ɨ�������socket����ȡ��Ӧ���ݣ���ִ����Ӧ����
		ret = scanIO();
		LOG_DEBUG("End worker intevel %d\n", i);
		i++;
		if(RET_NO_SOCKET_READY == ret && !stop)
		{
			delay_ms(WORKER_SLEEP_TIME);
		}
	}

	/**** worker����ֹ���˴�Ӧ���������� ****/
	LOG_DEBUG("Worker was stoped, begin cleaning job\n");
	//���������������
	for(i = 0; i < MAX_TASKQUE_SIZE; i++)
	{
		if(NULL != taskQue[i])
		{	
			clearTaskInfo(i);
		}
	}
	LOG_DEBUG("taskQue cleaned\n");
	//����map��
	s_t_map.clear();
	//TODO:����������
	LOG_DEBUG("s_t_map cleaned\n");
	
	return 0;
}


/******************************************************************************
 * TTransferWorker.clearTaskInfo - ���������������Ϣ�����ͷ�������ռ��������пռ� 
 * ---- ע�����ô˺�������ȷ����ȡ�� ������task_que_lock
 * DESCRIPTION: - 
 *    n/a 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 01dec2008, Davidfan written
 * --------------------
 ******************************************************************************/
void TTransferWorker::clearTaskInfo(int taskid)
{
	TTask* pTask = taskQue[taskid];
	if(NULL != pTask)
	{
		LOG_DEBUG("Clearing task=%d\n", taskid);
//			if(pTask->taskState > TSK_START)
//			{
//	#ifndef HTTPTRANSFER_RELEASE
//				pTask->dumpDataToFile();
//	#endif
//			}
		
		int sockfd = pTask->hcp->getSocket()->get_socketfd();
		
		//��socket�Ӽ������������
		FD_CLR(sockfd, &readfds);
		FD_CLR(sockfd, &writefds);
		//��socket��ӳ�����ɾ��
		s_t_map.erase(sockfd);

		//��socket�Ӽ������������

		//���ⲿ�̻߳�ȡpTaskָ�����ͬ������Ҫ������
		//�����ڵ��øú�����scanTaskQue�Ѿ��������ʴ˴��������---
		thread_lock(&task_que_lock);	
		delete pTask;		//����TASK��������������ڲ�������
		taskQue[taskid] = NULL;	
		thread_unlock(&task_que_lock);
		
	}
	else
	{
		LOG_DEBUG("Task=%d not exist when clearing\n", taskid);		
	}
}

TTransferWorker::TTransferWorker(UINT32 in_maxRunTaskNum)
{
	maxPos = 0;
	curPos = 0;
	maxRunTaskNum = in_maxRunTaskNum;
	stop = false;
	maxfd = 0;
	FD_ZERO(&readfds);	
	FD_ZERO(&writefds);
	
	for(int i = 0; i< MAX_TASKQUE_SIZE; i++)
	{
		taskQue[i] = NULL;
	}
	//memset(taskQue, 0x0, sizeof(taskQue)*sizeof(TTask*));
	runningTaskCount = 0;
	startingTaskCount = 0;
	waittingTaskCount = 0;
	finishTaskCount = 0;
	errorTaskCount = 0;
	unkownTaskCount = 0;
	connectingTaskCount = 0;

	thread_lock_init(&task_que_lock);
}

TTransferWorker::~TTransferWorker()
{
	thread_lock_uninit(&task_que_lock);
}




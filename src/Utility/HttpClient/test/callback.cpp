#include "test.h"

INT32 handle_init(INT32 taskid, const TaskInfo* taskinfo)
{
	printf("\n ******** handle_init ********\n");
	printf("taskid=%d contentLength=%d\n", taskid, taskinfo->contentLength);
	printf("taskid=%d doneSize=%d\n", taskid, taskinfo->doneSize);
	printf("taskid=%d transferSpeed=%d\n", taskid, taskinfo->transferSpeed);
	printf("taskid=%d remainTime=%d\n", taskid, taskinfo->remainTime);
	printf("taskid=%d addTime=%d\n", taskid, taskinfo->addTime);
	printf("taskid=%d startTime=%d\n", taskid, taskinfo->startTime);
	printf("taskid=%d endTime=%d\n", taskid, taskinfo->endTime);
	printf("taskid=%d errcode=%d\n", taskid, taskinfo->errcode);
	printf("taskid=%d httpCode=%d\n", taskid, taskinfo->httpCode);
	printf("taskid=%d infomsg=%s\n", taskid, taskinfo->infomsg);	
	printf("taskid=%d resq data size=%d\n ", taskid, taskinfo->respData->size());	
	printf("************************\n");
	return 0;
}

INT32 handle_headcomplete(INT32 taskid, const TaskInfo* taskinfo)
{
	printf("\n ******** handle_init ********\n");
	printf("taskid=%d contentLength=%d\n", taskid, taskinfo->contentLength);
	printf("taskid=%d doneSize=%d\n", taskid, taskinfo->doneSize);
	printf("taskid=%d transferSpeed=%d\n", taskid, taskinfo->transferSpeed);
	printf("taskid=%d remainTime=%d\n", taskid, taskinfo->remainTime);
	printf("taskid=%d addTime=%d\n", taskid, taskinfo->addTime);
	printf("taskid=%d startTime=%d\n", taskid, taskinfo->startTime);
	printf("taskid=%d endTime=%d\n", taskid, taskinfo->endTime);
	printf("taskid=%d errcode=%d\n", taskid, taskinfo->errcode);
	printf("taskid=%d httpCode=%d\n", taskid, taskinfo->httpCode);
	printf("taskid=%d infomsg=%s\n", taskid, taskinfo->infomsg);	
	printf("************************\n");
	return 0;
}

INT32 handle_process(INT32 taskid, const TaskInfo* taskinfo)
{
	printf("\n ******** handle_process ********\n");
	printf("taskid=%d contentLength=%d\n", taskid, taskinfo->contentLength);
	printf("taskid=%d doneSize=%d\n", taskid, taskinfo->doneSize);
	printf("taskid=%d transferSpeed=%d\n", taskid, taskinfo->transferSpeed);
	printf("taskid=%d remainTime=%d\n", taskid, taskinfo->remainTime);
	printf("taskid=%d addTime=%d\n", taskid, taskinfo->addTime);
	printf("taskid=%d startTime=%d\n", taskid, taskinfo->startTime);
	printf("taskid=%d endTime=%d\n", taskid, taskinfo->endTime);
	printf("taskid=%d errcode=%d\n", taskid, taskinfo->errcode);
	printf("taskid=%d httpCode=%d\n", taskid, taskinfo->httpCode);
	printf("taskid=%d infomsg=%s\n", taskid, taskinfo->infomsg);	
	printf("taskid=%d resq data size=%d\n ", taskid, taskinfo->respData == NULL ? 0xff:taskinfo->respData->size());	
	printf("************************\n");
	return 0;
}

INT32 handle_finish(INT32 taskid, const TaskInfo* taskinfo)
{
	printf("\n ******** handle_finish ********\n");
	printf("taskid=%d contentLength=%d\n", taskid, taskinfo->contentLength);
	printf("taskid=%d doneSize=%d\n", taskid, taskinfo->doneSize);
	printf("taskid=%d transferSpeed=%d\n", taskid, taskinfo->transferSpeed);
	printf("taskid=%d remainTime=%d\n", taskid, taskinfo->remainTime);
	printf("taskid=%d addTime=%d\n", taskid, taskinfo->addTime);
	printf("taskid=%d startTime=%d\n", taskid, taskinfo->startTime);
	printf("taskid=%d endTime=%d\n", taskid, taskinfo->endTime);
	printf("taskid=%d errcode=%d\n", taskid, taskinfo->errcode);
	printf("taskid=%d httpCode=%d\n", taskid, taskinfo->httpCode);
	printf("taskid=%d infomsg=%s\n", taskid, taskinfo->infomsg);
	printf("taskid=%d resq data size=%d\n ", taskid, taskinfo->respData == NULL ? 0xff:taskinfo->respData->size());	
	printf("************************\n");
	return 0;
}

INT32 handle_error(INT32 taskid, const TaskInfo* taskinfo)
{
	printf("\n ******** handle_error ********\n");
	printf("taskid=%d contentLength=%d\n", taskid, taskinfo->contentLength);
	printf("taskid=%d doneSize=%d\n", taskid, taskinfo->doneSize);
	printf("taskid=%d transferSpeed=%d\n", taskid, taskinfo->transferSpeed);
	printf("taskid=%d remainTime=%d\n", taskid, taskinfo->remainTime);
	printf("taskid=%d addTime=%d\n", taskid, taskinfo->addTime);
	printf("taskid=%d startTime=%d\n", taskid, taskinfo->startTime);
	printf("taskid=%d endTime=%d\n", taskid, taskinfo->endTime);
	printf("taskid=%d errcode=%d\n", taskid, taskinfo->errcode);
	printf("taskid=%d httpCode=%d\n", taskid, taskinfo->httpCode);
	printf("taskid=%d infomsg=%s\n", taskid, taskinfo->infomsg);	
	printf("************************\n");
	return 0;
}


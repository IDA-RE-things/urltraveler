#pragma  once

/**
 * ��ʼ���     
 */
extern INT32 handle_init(INT32 taskid, const TaskInfo* taskinfo);

/**
 * HTTP ͷ����ȡ������     
 */
extern INT32 handle_headcomplete(INT32 taskid, const TaskInfo* taskinfo);

/**
 * ��ȡHTTP���ݵĹ���     
 */
extern INT32 handle_process(INT32 taskid, const TaskInfo* taskinfo);

/**
 * ���ݶ�ȡ����     
 */
extern INT32 handle_finish(INT32 taskid, const TaskInfo* taskinfo);

/**
 * ��ȡ��ʱ�����˴���     
 */
extern INT32 handle_error(INT32 taskid, const TaskInfo* taskinfo);
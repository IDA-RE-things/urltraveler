#include "httpmanager.h"

extern INT32 handle_init(INT32 taskid, const TaskInfo* taskinfo);
extern INT32 handle_process(INT32 taskid, const TaskInfo* taskinfo);
extern INT32 handle_finish(INT32 taskid, const TaskInfo* taskinfo);
extern INT32 handle_error(INT32 taskid, const TaskInfo* taskinfo);
extern INT32 handle_headcomplete(INT32 taskid, const TaskInfo* taskinfo);



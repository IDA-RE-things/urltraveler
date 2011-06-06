#include "test.h"
#include "httpmanager.h"

#include <winsock2.h>

//char url[] = "http://www.ifeng.com";
char url_redirect[] = "http://pro.qq.com/?PushID=6716&MsgInfoID=14680";
char url1[] = "http://news.ifeng.com/mil/3/200811/1124_341_891677.shtml";
char url2[] = "http://news.ifeng.com/mil/2/200811/1124_340_891580.shtml";
char url3[] = "http://www.51.com";

char url_post[] = "http://szup.51.com/up_v2/PhotoUploadNormal.php?user=neptune119&type=Photo&key=3abd741d02edfea565eae45ed4648c8c&time=1227776620&vip=494a4a0e9e7f57dc5b0ae92f4d08274b";


CHAR_VECTOR outdata_redirect;
CHAR_VECTOR outdata_url1;
CHAR_VECTOR outdata_url2;
CHAR_VECTOR outdata_url3;

CHAR_VECTOR outdata_post;


void stop_test()
{
	int ret = 0;
	TranferManager* mgr = NULL;
	mgr = TranferManager::getInstance();
	mgr->start();

	POST_PARAMS params;
	char* paramName1 = "photname";
	char* paramValue1 = "yellowphoto";
	
	ret = params.add_param(paramName1, paramValue1, 
						  (UINT32)(strlen(paramValue1)+1), 
						  PARAM_TEXT);
	
	printf("add_param ret=%d.\n", ret);

	char* paramName2 = "upfile1";
	char* paramValue2 = "E:\\workfile\\testvpn.jpg";
	ret = params.add_param(paramName2, paramValue2, 
						  (UINT32)(strlen(paramValue2)+1), 
						  PARAM_FILENAME);
	
	printf("add_param ret=%d.\n", ret);

#if 1
	CALL_BACK_S callbacks ;
	callbacks.handle_init = handle_init;
	callbacks.handle_headcomplete = handle_headcomplete;
	callbacks.handle_process = handle_process;
	callbacks.handle_finish = handle_finish;
	callbacks.handle_error = handle_error;

	OPTIONS  opts;

	ret = mgr->addTask(url3, TASK_DOWNLOAD, 
		 NULL, 
		 callbacks, 
		 opts, NULL);
#endif


	
	printf("addTask ret=%d.\n", ret);

	printf("haha, wo jin lai le.\n");
	Sleep(50000);
	printf("program stop begin.\n");
	mgr->stop();
	printf("program stoped.\n");
	Sleep(100000);
	printf("program end.\n");


}


void redirect_test()
{
	int ret = 0;
	TranferManager* mgr = NULL;
	mgr = TranferManager::getInstance();
	mgr->start();


	//测试重定向功能
	CALL_BACK_S callbacks ;
	callbacks.handle_init = handle_init;
	callbacks.handle_headcomplete = handle_headcomplete;
	callbacks.handle_process = handle_process;
	callbacks.handle_finish = handle_finish;
	callbacks.handle_error = handle_error;

	OPTIONS  opts ;

	ret = mgr->addTask(url_redirect, TASK_DOWNLOAD, 
		 NULL,
		 callbacks, 
		 opts, NULL);	

	
	printf("addTask ret=%d.\n", ret);

	printf("haha, wo jin lai le.\n");
	Sleep(10000);
	mgr->stop();
	printf("program stoped.");
	Sleep(100000);
	printf("program end.");

}


void multitask_test()
{
	char *url = "http://wbsoft.wn51.com/wnwb_751.exe";
	int ret = 0;
	TranferManager* mgr = NULL;
	mgr = TranferManager::getInstance();
	mgr->start();

	//测试普通下载功能
	CALL_BACK_S callbacks;
	callbacks.handle_init = handle_init;
	callbacks.handle_headcomplete = handle_headcomplete;
	callbacks.handle_process = handle_process;
	callbacks.handle_finish = handle_finish;
	callbacks.handle_error = handle_error;

	OPTIONS  opts;


	ret = mgr->addTask(url1, TASK_DOWNLOAD, 
		 NULL, 
		 callbacks, 
		 opts, NULL);

	Sleep(100);
	//几个任务一同添加测试多任务下载功能
	ret = mgr->addTask(url2, TASK_DOWNLOAD, 
		 NULL, 
		 callbacks, 
		 opts, NULL);

	Sleep(1000);

	ret = mgr->addTask(url, TASK_DOWNLOAD, 
		 NULL, 
		 callbacks, 
		 opts, NULL);	
	

	printf("haha, wo jin lai le.\n");
	Sleep(1000000);
	mgr->stop();
	printf("program stoped.");
	Sleep(100000);
	printf("program end.");

}

void upload_test()
{
	int ret = 0;
	TranferManager* mgr = NULL;
	mgr = TranferManager::getInstance();
	mgr->start();
	
	POST_PARAMS params;
	char* paramName1 = "photname";
	char* paramValue1 = "yellowphoto";


	ret = params.add_param(paramName1, paramValue1, 
						  (UINT32)(strlen(paramValue1)+1), 
						  PARAM_TEXT);

	printf("add_param ret=%d.\n", ret);

	char* paramName2 = "upfile1";
	char* paramValue2 = "E:\\workfile\\testvpn.jpg";
	ret = params.add_param(paramName2, paramValue2, 
						  (UINT32)(strlen(paramValue2)+1), 
						  PARAM_FILENAME);

	printf("add_param ret=%d.\n", ret);


#if 1
	CALL_BACK_S callbacks;
	callbacks.handle_init = handle_init;
	callbacks.handle_headcomplete = handle_headcomplete;
	callbacks.handle_process = handle_process;
	callbacks.handle_finish = handle_finish;
	callbacks.handle_error = handle_error;

	OPTIONS  opts;


	ret = mgr->addTask(url_post, TASK_UPLOAD, 
		 &params, 
		 callbacks, 
		 opts, NULL);
#endif


	printf("addTask ret=%d.\n", ret);

	printf("haha, wo jin lai le.\n");
	Sleep(10000);
	mgr->stop();
	printf("program stoped.");
	Sleep(100000);
	printf("program end.");

}

void restart_test()
{
	stop_test();

	printf("stoped 1st \n");
	stop_test();

	printf("stoped 2nd \n");
}


void custom_header_download_test()
{
	//char *url = "http://wbsoft.wn51.com/wnwb_751.exe";
	//char *url = "http://www.wnwb.com/wnwb2005/images/tt98/88trip_ad540x51.gif";
	//char *url = "http://p5.images22.51img1.com/6000/neptune119/588f3d1788bf71c9ddcb7ebf024f0ad6.jpg";
	char *url = "http://down.yich.org/xuexi/086.mp3"; //---771555

	int ret = 0;
	TranferManager* mgr = NULL;
	mgr = TranferManager::getInstance();
	mgr->start();

	//测试普通下载功能
	CALL_BACK_S callbacks;
	callbacks.handle_init = handle_init;
	callbacks.handle_headcomplete = handle_headcomplete;
	callbacks.handle_process = handle_process;
	callbacks.handle_finish = handle_finish;
	callbacks.handle_error = handle_error;

	OPTIONS  opts;
	
	HttpHdrs*  pheaders = new HttpHdrs();

	pheaders->SetHdr("COOKIE", "xxxxxxxxxxxx");


	ret = mgr->addTask(url, TASK_DOWNLOAD, 
		 NULL, 
		 callbacks, 
		 opts, pheaders);

	printf("addTask ret=%d.\n", ret);

	printf("haha, wo jin lai le.\n");
	Sleep(1000000);
	mgr->stop();
	printf("program stoped.");
	Sleep(100000);
	printf("program end.");

}


void download_test()
{
//	char *url = "http://wbsoft.wn51.com/wnwb_751.exe";
	char *url = "http://bbs.xjtu.edu.cn";
	//char *url = "http://www.wnwb.com/wnwb2005/images/tt98/88trip_ad540x51.gif";
	//char *url = "http://p5.images22.51img1.com/6000/neptune119/588f3d1788bf71c9ddcb7ebf024f0ad6.jpg";
	//char *url = "http://down.yich.org/xuexi/086.mp3"; //---771555

	int ret = 0;
	TranferManager* mgr = NULL;
	mgr = TranferManager::getInstance();
	mgr->start();

	//测试普通下载功能
	CALL_BACK_S callbacks;
	callbacks.handle_init = handle_init;
	callbacks.handle_headcomplete = handle_headcomplete;
	callbacks.handle_process = handle_process;
	callbacks.handle_finish = handle_finish;
	callbacks.handle_error = handle_error;

	OPTIONS  opts;
	

	ret = mgr->addTask(url, TASK_DOWNLOAD, 
		 NULL, 
		 callbacks, 
		 opts, NULL);

	printf("addTask ret=%d.\n", ret);

	printf("haha, wo jin lai le.\n");
	Sleep(1000000);
	mgr->stop();
	printf("program stoped.");
	Sleep(100000);
	printf("program end.");

}

void partdownload_test()
{
	char *url = url1;
	//char *url = "http://www.wnwb.com/wnwb2005/images/tt98/88trip_ad540x51.gif";
	//char *url = "http://p5.images22.51img1.com/6000/neptune119/588f3d1788bf71c9ddcb7ebf024f0ad6.jpg";
	//char *url = "http://down.yich.org/xuexi/086.mp3"; //---771555

	int ret = 0;
	TranferManager* mgr = NULL;
	mgr = TranferManager::getInstance();
	mgr->start();

	//测试普通下载功能
	CALL_BACK_S callbacks;
	callbacks.handle_init = handle_init;
	callbacks.handle_headcomplete = handle_headcomplete;
	callbacks.handle_process = handle_process;
	callbacks.handle_finish = handle_finish;
	callbacks.handle_error = handle_error;

	OPTIONS  opts;
	opts.startSize = 10000;	

	ret = mgr->addTask(url, TASK_DOWNLOAD, 
		 NULL, 
		 callbacks, 
		 opts, NULL);

	printf("addTask ret=%d.\n", ret);

	printf("haha, wo jin lai le.\n");
	Sleep(1000000);
	mgr->stop();
	printf("program stoped.");
	Sleep(100000);
	printf("program end.");

}

void dataCleanDownload_test()
{
	char *url = "http://wbsoft.wn51.com/wnwb_751.exe";
	//char *url = "http://www.wnwb.com/wnwb2005/images/tt98/88trip_ad540x51.gif";
	//char *url = "http://p5.images22.51img1.com/6000/neptune119/588f3d1788bf71c9ddcb7ebf024f0ad6.jpg";
	//char *url = "http://down.yich.org/xuexi/086.mp3"; //---771555

	int ret = 0;
	TranferManager* mgr = NULL;
	mgr = TranferManager::getInstance();
	mgr->start();

	//测试普通下载功能
	CALL_BACK_S callbacks;
	callbacks.handle_init = handle_init;
	callbacks.handle_headcomplete = handle_headcomplete;
	callbacks.handle_process = handle_process;
	callbacks.handle_finish = handle_finish;
	callbacks.handle_error = handle_error;

	OPTIONS  opts;
	opts.dataClean = true;	

	ret = mgr->addTask(url, TASK_DOWNLOAD, 
		 NULL, 
		 callbacks, 
		 opts, NULL);

	printf("addTask ret=%d.\n", ret);

	printf("haha, wo jin lai le.\n");
	Sleep(1000000);
	mgr->stop();
	printf("program stoped.");
	Sleep(100000);
	printf("program end.");

}

void timeout_test()
{
	char *url = "http://wbsoft.wn51.com/wnwb_751.exe";

	int ret = 0;
	TranferManager* mgr = NULL;
	mgr = TranferManager::getInstance();
	mgr->start();

	//测试普通下载功能
	CALL_BACK_S callbacks;
	callbacks.handle_init = handle_init;
	callbacks.handle_headcomplete = handle_headcomplete;
	callbacks.handle_process = handle_process;
	callbacks.handle_finish = handle_finish;
	callbacks.handle_error = handle_error;

	OPTIONS  opts;
	opts.tasktimeout = 1;

	ret = mgr->addTask(url1, TASK_DOWNLOAD, 
		 NULL, 
		 callbacks, 
		 opts, NULL);

	printf("addTask ret=%d.\n", ret);

	printf("haha, wo jin lai le.\n");
	Sleep(10000);
	mgr->stop();
	printf("program stoped.");
	Sleep(100000);
	printf("program end.");

}

void httperror_test()
{
	char url_httperror[] = "http://news.ifeng.com/xxxxxxx.shtml";
	int ret = 0;
	TranferManager* mgr = NULL;
	mgr = TranferManager::getInstance();
	mgr->start();

	//测试普通下载功能

	CALL_BACK_S callbacks ;
	OPTIONS  opts ;
	
	ret = mgr->addTask(url_httperror, TASK_DOWNLOAD, 
		 NULL, 
		 callbacks, 
		 opts, NULL);

	printf("addTask ret=%d.\n", ret);

	printf("haha, wo jin lai le.\n");
	Sleep(10000);
	mgr->stop();
	printf("program stoped.");
	Sleep(100000);
	printf("program end.");

}

void sock5_withPass_download_test()
{
	char *url = "http://wbsoft.wn51.com/wnwb_751.exe";
	//char *url = "http://www.wnwb.com/wnwb2005/images/tt98/88trip_ad540x51.gif";
	//char *url = "http://p5.images22.51img1.com/6000/neptune119/588f3d1788bf71c9ddcb7ebf024f0ad6.jpg";
	//char *url = "http://down.yich.org/xuexi/086.mp3"; //---771555

	int ret = 0;
	TranferManager* mgr = NULL;
	Proxy_t proxy;
	proxy.host = "192.168.124.128";
	proxy.port = 1080; 
	proxy.type = PROXY_SOCK5;
	proxy.user = "fanxing";
	proxy.pwd  = "fanxing123";
	
	mgr = TranferManager::getInstance();

	ret = mgr->setProxy(proxy);
	printf("setProxy ret=%d\n", ret);

	ret = mgr->start();
	printf("start ret=%d\n", ret);

	//测试普通下载功能
	CALL_BACK_S callbacks;
	callbacks.handle_init = handle_init;
	callbacks.handle_headcomplete = handle_headcomplete;
	callbacks.handle_process = handle_process;
	callbacks.handle_finish = handle_finish;
	callbacks.handle_error = handle_error;

	OPTIONS  opts;
	

	ret = mgr->addTask(url, TASK_DOWNLOAD, 
		 NULL, 
		 callbacks, 
		 opts, NULL);

	printf("addTask ret=%d.\n", ret);

	printf("haha, wo jin lai le.\n");
	Sleep(1000000);
	mgr->stop();
	printf("program stoped.");
	Sleep(100000);
	printf("program end.");

}

void sock5_withoutPass_download_test()
{
	char *url = "http://wbsoft.wn51.com/wnwb_751.exe";
	//char *url = "http://www.wnwb.com/wnwb2005/images/tt98/88trip_ad540x51.gif";
	//char *url = "http://p5.images22.51img1.com/6000/neptune119/588f3d1788bf71c9ddcb7ebf024f0ad6.jpg";
	//char *url = "http://down.yich.org/xuexi/086.mp3"; //---771555

	int ret = 0;
	TranferManager* mgr = NULL;
	Proxy_t proxy;
	proxy.host = "192.168.124.128";
	proxy.port = 1080; 
	proxy.type = PROXY_SOCK5;
//	proxy.user = "fanxing";
//	proxy.pwd  = "fanxing123";
	
	mgr = TranferManager::getInstance();

	ret = mgr->setProxy(proxy);
	printf("setProxy ret=%d\n", ret);

	ret = mgr->start();
	printf("start ret=%d\n", ret);

	//测试普通下载功能
	CALL_BACK_S callbacks;
	callbacks.handle_init = handle_init;
	callbacks.handle_headcomplete = handle_headcomplete;
	callbacks.handle_process = handle_process;
	callbacks.handle_finish = handle_finish;
	callbacks.handle_error = handle_error;

	OPTIONS  opts;
	

	ret = mgr->addTask(url, TASK_DOWNLOAD, 
		 NULL, 
		 callbacks, 
		 opts, NULL);

	printf("addTask ret=%d.\n", ret);

	printf("haha, wo jin lai le.\n");
	Sleep(1000000);
	mgr->stop();
	printf("program stoped.");
	Sleep(100000);
	printf("program end.");

}

void http_withoutPass_download_test()
{
	char *url = "http://wbsoft.wn51.com/wnwb_751.exe";
	//char *url = "http://www.wnwb.com/wnwb2005/images/tt98/88trip_ad540x51.gif";
	//char *url = "http://p5.images22.51img1.com/6000/neptune119/588f3d1788bf71c9ddcb7ebf024f0ad6.jpg";
	//char *url = "http://down.yich.org/xuexi/086.mp3"; //---771555

	int ret = 0;
	TranferManager* mgr = NULL;
	Proxy_t proxy;
	proxy.host = "192.168.52.155";
	proxy.port = 8080; 
	proxy.type = PROXY_HTTP;
//	proxy.user = "fanxing";
//	proxy.pwd  = "fanxing123";
	
	mgr = TranferManager::getInstance();

	ret = mgr->setProxy(proxy);
	printf("setProxy ret=%d\n", ret);

	ret = mgr->start();
	printf("start ret=%d\n", ret);

	//测试普通下载功能
	CALL_BACK_S callbacks;
	callbacks.handle_init = handle_init;
	callbacks.handle_headcomplete = handle_headcomplete;
	callbacks.handle_process = handle_process;
	callbacks.handle_finish = handle_finish;
	callbacks.handle_error = handle_error;

	OPTIONS  opts;
	

	ret = mgr->addTask(url, TASK_DOWNLOAD, 
		 NULL, 
		 callbacks, 
		 opts, NULL);

	printf("addTask ret=%d.\n", ret);

	printf("haha, wo jin lai le.\n");
	Sleep(1000000);
	mgr->stop();
	printf("program stoped.");
	Sleep(100000);
	printf("program end.");

}

void http_withPass_download_test()
{
	char *url = "http://wbsoft.wn51.com/wnwb_751.exe";
	//char *url = "http://www.wnwb.com/wnwb2005/images/tt98/88trip_ad540x51.gif";
	//char *url = "http://p5.images22.51img1.com/6000/neptune119/588f3d1788bf71c9ddcb7ebf024f0ad6.jpg";
	//char *url = "http://down.yich.org/xuexi/086.mp3"; //---771555

	int ret = 0;
	TranferManager* mgr = NULL;
	Proxy_t proxy;
	proxy.host = "192.168.52.155";
	proxy.port = 8080; 
	proxy.type = PROXY_HTTP;
	proxy.user = "test";
	proxy.pwd  = "test123";
	
	mgr = TranferManager::getInstance();

	ret = mgr->setProxy(proxy);
	printf("setProxy ret=%d\n", ret);

	ret = mgr->start();
	printf("start ret=%d\n", ret);

	//测试普通下载功能
	CALL_BACK_S callbacks;
	callbacks.handle_init = handle_init;
	callbacks.handle_headcomplete = handle_headcomplete;
	callbacks.handle_process = handle_process;
	callbacks.handle_finish = handle_finish;
	callbacks.handle_error = handle_error;

	OPTIONS  opts;
	

	ret = mgr->addTask(url, TASK_DOWNLOAD, 
		 NULL, 
		 callbacks, 
		 opts, NULL);

	printf("addTask ret=%d.\n", ret);

	printf("haha, wo jin lai le.\n");
	Sleep(1000000);
	mgr->stop();
	printf("program stoped.");
	Sleep(100000);
	printf("program end.");

}


void main()
{
	//restart_test();
	
//	stop_test();
	download_test();
//	multitask_test();
//	partdownload_test();
//	dataCleanDownload_test();
//	httperror_test();
//	sock5_withPass_download_test();		//test pass with ss5
//	sock5_withoutPass_download_test();		//test pass with ss5
//	http_withoutPass_download_test();		//test pass ccproxy
//	http_withPass_download_test();	//test pass ccproxy
//	custom_header_download_test();	

	printf("Test end \n");
}

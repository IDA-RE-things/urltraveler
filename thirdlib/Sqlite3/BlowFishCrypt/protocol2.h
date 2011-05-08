

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

/*********************/
/*验证服务器相关协议 */
/*********************/

const int MAX_CLIENT_MSG_LEN = 2048;
const int MAX_SERVER_MSG_LEN = 2048;
const int MAX_HTTPTAG_LEN = 1024;
const int MAX_HEAD_OPTION_LEN = 128;
const int MAX_SIGNATURE_LEN = 128;
const int MAX_ERRMSG_LEN = 256;
const int MAX_URL_LEN = 256;

//CS
enum MsgID
{
    MSG_AUTHRIZE_REQ = 1,
    MSG_AUTHRIZE_RES = 2
};

struct PkgHead
{
    short httptaglen;
    char httptag[MAX_HTTPTAG_LEN];
    short totallen;
    short headlen;
    short msgid;
    short version;
    char flag[2];
    short optionlen;
    char option[MAX_HEAD_OPTION_LEN];
};

struct AuthReq
{
    char uidlen;
    char uid[64];
    int passwordlen;
    char password[256];
};

struct AuthResSuc
{
    short result;
    int uin;
    char sessionkey[16];
    char siglen;
    char Signature[MAX_SIGNATURE_LEN];
};

struct Signature
{
    char sessionkey[16];
    int uin;
    char bitmap[16];
    int timestamp;
};

struct AuthResFail
{
    short result;
    char errmsg[MAX_ERRMSG_LEN];
    char url[MAX_ERRMSG_LEN]; 
};

//SS
enum
{
	CMD_DB_KEEPALIVE = 0,
	CMD_DB_AUTH_USER,
	CMD_GET_PASSWORD,
};

enum 
{
    //后台返回
	LOGIN_SUCCESS		= 0, //获取成功
	LOGIN_AUTH_SERVER_DISCONNECTED = 20,		//后台验证服务器错误
	LOGIN_NO_USER = 22,						// 没有该用户

    //本机生成
    LOGIN_AUTH_SERVER_BUSY = 30, //连接后台服务错误
    LOGIN_PASSWORD_ERR = 31, //密码验证 错误
};

#endif


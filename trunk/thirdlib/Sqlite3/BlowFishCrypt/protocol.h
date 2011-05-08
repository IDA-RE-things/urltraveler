

#pragma  once

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
const int MAX_UID_LEN = 64;
const int MAX_PSWD_LEN = 64;

//CS
enum MsgID
{
    MSG_AUTHRIZE_REQ = 1,
    MSG_AUTHRIZE_RES = 2,
    MSG_SIGNTRANS_REQ = 3,
    MSG_SIGNTRANS_RES = 4
};

struct PkgHead
{
    /*
    short httptaglen;
        char httptag[MAX_HTTPTAG_LEN];*/
    char tag;
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
    char uid[MAX_UID_LEN];
    short appid;
	short passwordlen;
    char password[MAX_PSWD_LEN];
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
    char uidlen;
    char uid[MAX_UID_LEN];
    char bitmap[16];
    int timestamp;
};

struct AuthResFail
{
    short result;
    char errmsg[MAX_ERRMSG_LEN];
    char url[MAX_URL_LEN]; 
};

struct SignTransReq
{
    short source_appid;
    short target_appid;
    char siglen;
    char Signature[MAX_SIGNATURE_LEN];    
};

struct SignTransSuc
{
    short result;
    char siglen;
    char Signature[MAX_SIGNATURE_LEN];

};

struct SignTransFail
{
    short result;
    char errmsg[MAX_ERRMSG_LEN];
    char url[MAX_URL_LEN]; 
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

    TRANS_SOURCEAPPID_ERR = 50,  //错误的源业务id 或者版本号
    TRANS_TARGETAPPID_ERR = 51,  //错误的目标业务id或者版本号
    TRANS_DECRYPT_ERR = 52, //解密错误
    TRANS_ENCRYPT_ERR = 53, //加密错误
};

//本服务访问后台验证服务请求
struct GetMd5Req
{
    char account[MAX_UID_LEN];
};

//后台验证服务返回
struct GetMd5Res
{
    char res;
    int uin;
    char account[MAX_UID_LEN];
    char md5[33];
};



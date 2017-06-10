#ifndef __ZQQ_PROTOCOL__H
#define __ZQQ_PROTOCOL__H

//#define LOGIN_PKT			0
//#define LOGOUT_PKT			1
//#define LOGIN_REPLY_PKT		2
//#define PUBLIC_CHAT_PKT		3
//#define PRIVATE_CHAT_PKT	4

#define LOGIN_PKT			100
#define LOGOUT_PKT			101
#define LOGIN_REPLY_PKT		102
#define PUBLIC_CHAT_PKT		103
#define PRIVATE_CHAT_PKT	104
#define UPDATE_ONLINEUSER_PKT	105
#define WINDOWSSHAKING_PKT	    106
#define TRANSMITFILECMD_PKT     107


#define PUBLIC_CHAT_HEAD    (sizeof(Header) + 20)
#define PRIVATE_CHAT_HEAD   (sizeof(Header) + 40)
#define RE_ONLINEUSER_HEAD  (sizeof(Header) + 8)
#define TRANSMITFILE_HEAD   (sizeof(Header) + 84)

typedef struct Header
{
	int len;
	int type;
}Header;

typedef struct LoginPkt
{
	Header header;
	char   userName[20];
	char   password[20];
}LoginPkt;

typedef struct LogoutPkt
{
	Header header;
	char   userName[20];
}LogoutPkt;

#define LOGIN_FAIL	  0
#define LOGIN_SUCCESS 1
typedef struct LoginRelpyPkt
{
	Header header;
	int		retCode;
}LoginRelpyPkt;

typedef struct PublicChatMsg
{
	Header header;
	char   from[20];
	char   message[512];
}PublicChatMsg;

typedef struct PrivateChatMsg
{
	Header header;
	char   from[20];
	char   to[20];
	char   message[512];
}PrivateChatMsg;

//获取在线用户名
typedef struct RequestOnlineUserPkt
{
	Header header;
	int ReqNumber;
}RequestOnlineUserPkt;

//服务端返回在线用户名
typedef struct ResponseOnlineUserPkt
{
	Header header;
	int OnlineNum;     //在线用户数
	int flag;          //保留（如果在线用户太多再用）
	char users[1024];  //在线用户名字（用'\0'分割）
}ResponsetOnlineUserPkt;

//窗口抖动
typedef struct WindowShakingPkt
{
	Header header;
	char   from[20];
	char   to[20];
}WindowShakingPkt;

/*
发文件：
1.客户端发出FILE_SEND
2.服务端根据情况发出FILE_RECV/ FILE_EXIST/ FILE_SERVERERROR
(1)客户端收到FILE_RECV，打开数据端口发FILE_SEND，并开始传文件
(2)客户端收到FILE_EXIST，提示发送成功
(3)客户端收到FILE_SERVERERROR，提示发送失败
3.文件传输结束时关闭数据端口

收文件：
1.服务端发出FILE_SEND
2.客户端收到FILE_SEND，打开数据端口发FILE_RECV
3.服务端在数据端口收到FILE_RECV，就开始发文件
4.文件传输结束时关闭数据端口
5.客户端发出FILE_RECVSUCCESS
6.服务端把FILE_RECVSUCCESS转发给发送方
7.发送方提示对方已收到文件
*/
//这里是传文件包命令字段的定义
#define FILE_SEND         101  //请求发送文件
#define FILE_RECV         102  //请求接收文件
#define FILE_RECVSUCCESS  200  //接收文件成功
#define FILE_EXIST        304  //文件已经存在
#define FILE_NOTFOUND     404  //文件已经过期
#define FILE_SERVERERROR  500  //服务端不想收

//在命令端口的包格式
typedef struct TransmitFlieCmdPkt
{
	Header header;
	int    cmd;       //命令字
	int    fileSize;  //文件大小
	int    recvSize;  //保留做断点续传用
	char   from[20];
	char   to[20];
	char   md5[32];   //32位MD5校验码
	char   filename[256];
}TransmitFlieCmdPkt;

//在数据端口的包格式(在发数据之前发)
//这里的cmd只能用FILE_SEND和FILE_RECV
typedef struct FileHeader
{
	int    cmd;       //命令字（收or发）
	int    fileSize;  //文件大小
	int    recvSize;  //保留做断点续传用
	char   from[20];
	char   to[20];  
	char   md5[32];   //32位MD5校验码
}FileHeader;

#endif //__ZQQ_PROTOCOL__H

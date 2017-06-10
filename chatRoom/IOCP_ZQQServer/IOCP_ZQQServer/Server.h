#pragma once

#include <winsock2.h>
#include "Protocol.h"
#include "myQueue.h"

typedef enum
{
	OPERATION_NULL,
	OPERATION_ACCEPT,
	OPERATION_RECV,
	OPERATION_SEND
}OPERATION_TYPE;

#define DATABUFFSIZE  1024
#define FILEBUFFSIZE  4096

//重叠I/O需要用到的结构体，临时记录IO数据
typedef struct
{
	OVERLAPPED overlapped;
	OPERATION_TYPE operationType;
	WSABUF databuff;
	char buffer[DATABUFFSIZE];
	DWORD bufferLen;
	DWORD flags;
	SOCKET socket;
}PER_IO_DATA, *LPPER_IO_DATA;

//收文件用的结构体
typedef struct
{
	OVERLAPPED overlapped;
	OPERATION_TYPE operationType;
	WSABUF databuff;
	char buffer[FILEBUFFSIZE];
	DWORD bufferLen;
	DWORD flags;
	SOCKET socket;
	SOCKET fsockfrom;
	SOCKET fsockto;
	int fileSize;
	int recvSize;
	int state;     //传文件的状态
	char md5[36];   //32位MD5校验码
}PER_FILE_DATA, *LPPER_FILE_DATA;

//#define STATE_WAITRECV  10
//#define STATE_RECV      11
//#define STATE_WAITSEND  12
//#define STATE_SEND      13
#define STATE_WAITHEAD  14
#define STATE_END       15
#define STATE_START     16
#define STATE_WAITTRANS 17
#define STATE_TRANS     18

typedef struct FileInformation
{  
	SOCKET sockData;  //数据端口
	int    state;     //传文件的状态
	int    fileSize;  //文件大小
	SOCKET fsockfrom;
	SOCKET fsockto;
	char   from[20];
	char   to[20];
	char   md5[36];   //32位MD5校验码
	char   filename[256];
}FileInformation;


typedef struct User
{
	char name[20];
	char password[20];
}User;

#define MAX_USER_NUM 63

#define MAX_FILE_NUM 64
//User g_userDB[MAX_USER_NUM];
//int  g_userNum = 0;

#define UNKNOWN_PKT_FORMAT	-1

#define PASSWORD_ERROR		-1
#define USER_UNFOUND		0
#define USER_FOUND			1

#define OPEN_USER_DB_FAIL	-1

//#define WSAERROR(s) printf("%s %d %s error : %d\n", __FILE__, __LINE__, s, WSAGetLastError())
#define MYERROR(s) //printf("%s %d %s error : %d\n", __FILE__, __LINE__, s, GetLastError())


typedef struct Session
{
	char userName[20];
	SOCKET sock;
	int state;
	Queue queue;
}Session;

void processSocketData(int sock);

int processPkt(const char* pkt, Session* session);
int processLoginPkt(const char* pkt, Session* session);
int processLogoutPkt(const char* pkt, Session* session);
int processPublicChatPkt(const char* pkt);
int processPrivateChatPkt(const char *pkt, Session* session);
int processTransmitFileCmdPkt(const char *pkt, Session *session);
int processWindowShakingPkt(const char *pkt, Session *session);
int processUpdateOnlineUserPkt(const char *pkt, Session *session);

int broadcastPkt(PublicChatMsg* publicChatMsg);

int checkUser(const User* userDB, int userNum, const User* user);
int loadUserDB(const char* fileName);

#define STATE_UNLOGIN 0
#define STATE_LOGIN	  1

#define UNFOUND_SESSION 0
#define REMOVE_SESSION_SUCCESS 1

#define FIND_SESSION_FAIL 0
#define FIND_SESSION_SUCCESS 1

//Session g_online_sessions[MAX_USER_NUM];
//int     g_online_session_num = 0;

Session *addSession(SOCKET sNewConnection);
int findSocket(const char* userName, SOCKET* sock);
int session_queue_Write(SOCKET sock, const char *buff, int length);
//int removeSession(const char* userName);
int removeSession(SOCKET sock);

int addFileTable(TransmitFlieCmdPkt transmitFileCmdPkt);
void clearFileTable();

int processFileHeader(SOCKET sock, SOCKET *sockto, FileHeader fileHeader);
int transmitEnd(LPPER_FILE_DATA lpPerFileData);

int myWSASend(SOCKET s, const char *buf, int len, DWORD flags);
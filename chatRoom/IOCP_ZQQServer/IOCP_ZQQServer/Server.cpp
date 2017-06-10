#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>

#include "Protocol.h"
#include "Server.h"


User g_userDB[MAX_USER_NUM];
int  g_userNum = 0;

Session g_online_sessions[MAX_USER_NUM];
int     g_online_session_num = 0;

FileInformation g_fileInformation[MAX_FILE_NUM];
int             g_fileInformation_num = 0;
HANDLE          g_fileInformation_mutex;


//处理套接字的数据
void processSocketData(int sock)
{
	int i, len;
	char buff[2048];
	for (i = 0;i < g_online_session_num;i++)
	{
		if (g_online_sessions[i].sock == sock)
			break;
	}
	if (0 > queue_Front(g_online_sessions[i].queue, buff, sizeof(len)))
		return;
	len = ntohl(((Header*)buff)->len);
	if (len > 1024 || len <= 0)
	{
		queue_Read(g_online_sessions[i].queue, buff, g_online_sessions[i].queue->length);
		return;
	}

	while (len <= g_online_sessions[i].queue->length)
	{
		int ret = queue_Read(g_online_sessions[i].queue, buff, len + sizeof(int));
		if (ret == len + sizeof(int))
			processPkt(buff, &g_online_sessions[i]);
		else
			break;
		memset(buff, 0, sizeof(buff));

		if (0 > queue_Front(g_online_sessions[i].queue, buff, sizeof(len)))
			break;
		len = ntohl(((Header*)buff)->len);
	}

}

int processPkt(const char* pkt, Session* session)
{
	((Header*)pkt)->len = ntohl(((Header*)pkt)->len);
	((Header*)pkt)->type = ntohl(((Header*)pkt)->type);

	int type = ((Header*)pkt)->type;
	//如果是登录包，做登录处理
	if (session->state == STATE_UNLOGIN && LOGIN_PKT != type)
	{
		return UNKNOWN_PKT_FORMAT;
	}

	if (LOGIN_PKT == type)
	{
		//return processLoginPkt(pkt, sock, addr);
		return processLoginPkt(pkt, session);
	}
	else if (LOGOUT_PKT == type)//否则如果是登出包，做登出处理
	{
		//return processLogoutPkt(pkt);
		return processLogoutPkt(pkt, session);
	}
	else if (PUBLIC_CHAT_PKT == type)//否则如果是公聊包，做公聊处理
	{
		//return processPublicChatPkt(pkt, sock);
		return processPublicChatPkt(pkt);
	}
	else if (PRIVATE_CHAT_PKT == type)//否则如果是私聊包，做私聊处理
	{
		return processPrivateChatPkt(pkt, session);
	}
	else if (WINDOWSSHAKING_PKT == type) //窗口抖动包
	{
		return processWindowShakingPkt(pkt, session);
	}
	else if (UPDATE_ONLINEUSER_PKT == type) //获取在线列表包
	{
		return processUpdateOnlineUserPkt(pkt, session);
	}
	else if (TRANSMITFILECMD_PKT == type)//否则如果是文件命令包，做文件命令处理
	{
		return processTransmitFileCmdPkt(pkt, session);
	}
	else //否则，错误处理
	{
		return UNKNOWN_PKT_FORMAT;
	}

}

int processLoginPkt(const char* pkt, Session* session)
{
	User user;
	LoginPkt* loginPkt = (LoginPkt*)pkt;
	strcpy(user.name, loginPkt->userName);
	strcpy(user.password, loginPkt->password);
	if (USER_FOUND == checkUser(g_userDB, g_userNum, &user))
	{
		LoginRelpyPkt replyPkt;
		replyPkt.header.len = htonl(sizeof(LoginRelpyPkt) - sizeof(int));
		replyPkt.header.type = htonl(LOGIN_REPLY_PKT);
		replyPkt.retCode = htonl(LOGIN_SUCCESS);
		myWSASend(session->sock, (const char*)&replyPkt, sizeof(replyPkt), 0);

		PublicChatMsg publicChatMsg;
		publicChatMsg.header.type = PUBLIC_CHAT_PKT;
		strcpy(publicChatMsg.from, "system");
		sprintf(publicChatMsg.message, "%s login!!!Welcome!!!!!!!!", loginPkt->userName);
		publicChatMsg.header.len = PUBLIC_CHAT_HEAD - 4 + strlen(publicChatMsg.message) + 1;
		broadcastPkt(&publicChatMsg);

		strcpy(session->userName, loginPkt->userName);
		session->state = STATE_LOGIN;

		//addSession(loginPkt->userName, sNewConnection);
	}
	else
	{
		LoginRelpyPkt replyPkt;
		replyPkt.header.type = htonl(LOGIN_REPLY_PKT);
		replyPkt.header.len = htonl(sizeof(replyPkt) - sizeof(int));
		replyPkt.retCode = htonl(LOGIN_FAIL);
		myWSASend(session->sock, (const char*)&replyPkt, sizeof(replyPkt), 0);
	}

	return 0;
}

int processLogoutPkt(const char* pkt, Session* session)
{
	LogoutPkt* logoutPkt = (LogoutPkt*)pkt;
	//removeSession(logoutPkt->userName);
	//session->state = STATE_UNLOGIN;
	removeSession(session->sock);
	return 0;
}

int processPublicChatPkt(const char* pkt)
{
	return broadcastPkt((PublicChatMsg*)pkt);
}

int processPrivateChatPkt(const char *pkt, Session *session)
{
	PrivateChatMsg *PrivatePkt = (PrivateChatMsg *)pkt;
	SOCKET sockTo;
	if (FIND_SESSION_SUCCESS == findSocket(PrivatePkt->to, &sockTo))
	{
		PrivatePkt->header.len = htonl(PrivatePkt->header.len);
		PrivatePkt->header.type = htonl(PrivatePkt->header.type);
		//myWSASend(session->sock, pkt, PRIVATE_CHAT_HEAD + strlen(PrivatePkt->message) + 1, 0);
		myWSASend(sockTo, pkt, PRIVATE_CHAT_HEAD + strlen(PrivatePkt->message) + 1, 0);
	}
	return 0;
}

int processWindowShakingPkt(const char *pkt, Session *session)
{
	WindowShakingPkt *windowShakingPkt = (WindowShakingPkt *)pkt;
	SOCKET sockTo;
	if (FIND_SESSION_SUCCESS == findSocket(windowShakingPkt->to, &sockTo))
	{
		windowShakingPkt->header.len = htonl(windowShakingPkt->header.len);
		windowShakingPkt->header.type = htonl(windowShakingPkt->header.type);
		//myWSASend(session->sock, pkt, sizeof(WindowShakingPkt), 0);
		myWSASend(sockTo, pkt, sizeof(WindowShakingPkt), 0);
	}
	return 0;
}

int processUpdateOnlineUserPkt(const char *pkt, Session *session)
{
	ResponseOnlineUserPkt rouPkt;
	int offset = 0;
	int len = 0;
	rouPkt.flag = 0;
	rouPkt.OnlineNum = 0;
	memset(rouPkt.users, 0, sizeof(rouPkt.users));

	for (int i = 0; i < g_online_session_num;i++)
	{
		if (STATE_LOGIN == g_online_sessions[i].state)
		{
			strcpy(rouPkt.users + offset, g_online_sessions[i].userName);
			offset += strlen(g_online_sessions[i].userName) + 1;
			rouPkt.users[offset - 1] = '\n';
			rouPkt.OnlineNum++;
		}
	}
	len = RE_ONLINEUSER_HEAD + offset;
	rouPkt.header.type = htonl(UPDATE_ONLINEUSER_PKT);
	rouPkt.header.len = htonl(len);
	rouPkt.flag = htonl(rouPkt.flag);
	rouPkt.OnlineNum = htonl(rouPkt.OnlineNum);
	
	myWSASend(session->sock, (const char *)&rouPkt, len, 0);
	return 0;
}

int processTransmitFileCmdPkt(const char *pkt, Session *session)
{
	TransmitFlieCmdPkt *transmitFileCmdPkt = (TransmitFlieCmdPkt *)pkt;
	transmitFileCmdPkt->cmd = ntohl(transmitFileCmdPkt->cmd);
	transmitFileCmdPkt->fileSize = ntohl(transmitFileCmdPkt->fileSize);
	transmitFileCmdPkt->recvSize = ntohl(transmitFileCmdPkt->recvSize);
	int ret = 0;
	int cmd = transmitFileCmdPkt->cmd;
	if (FILE_SEND == cmd)//客户端想发文件
	{
		ret = addFileTable(*transmitFileCmdPkt);//添加到待传输文件列表

		//创建响应包
		TransmitFlieCmdPkt replyPkt = *transmitFileCmdPkt;
		int pktlen = replyPkt.header.len+4;
		replyPkt.fileSize = htonl(replyPkt.fileSize);
		replyPkt.recvSize = htonl(replyPkt.recvSize);
		replyPkt.header.len = htonl(replyPkt.header.len);
		replyPkt.header.type = htonl(replyPkt.header.type);

		if (-FILE_SERVERERROR == ret)//文件列表满了
		{
			replyPkt.cmd = htonl(FILE_SERVERERROR);
			myWSASend(session->sock, (const char *)&replyPkt, pktlen, 0);
		}
		else//同意客户端发送文件
		{
			replyPkt.cmd = htonl(FILE_SEND);
			SOCKET sockto;
			findSocket(replyPkt.to, &sockto);
			myWSASend(sockto, (const char *)&replyPkt, pktlen, 0);
		}
	}
	else if (FILE_RECVSUCCESS == cmd)//文件成功接收提示
	{
		//创建响应包
		TransmitFlieCmdPkt replyPkt = *transmitFileCmdPkt;
		int pktlen = replyPkt.header.len + 4;
		replyPkt.fileSize = htonl(replyPkt.fileSize);
		replyPkt.recvSize = htonl(replyPkt.recvSize);
		replyPkt.header.len = htonl(replyPkt.header.len);
		replyPkt.header.type = htonl(replyPkt.header.type);
		SOCKET sockfrom;
		findSocket(replyPkt.from, &sockfrom);
		//给发送方回应
		replyPkt.cmd = htonl(FILE_RECVSUCCESS);
		myWSASend(sockfrom, (const char *)&replyPkt, pktlen, 0);
	}
	return 0;
}

int checkUser(const User* userDB, int userNum, const User* user)
{
	for (int i = 0; i<userNum; i++)
	{
		if (strcmp(userDB[i].name, user->name) == 0)
		{
			if (strcmp(userDB[i].password, user->password) == 0)
			{
				return USER_FOUND;
			}
			else
			{
				return PASSWORD_ERROR;
			}
		}
	}
	return USER_UNFOUND;
}

int loadUserDB(const char* fileName)
{
	FILE* fp = fopen(fileName, "r");
	if (NULL == fp)
		return OPEN_USER_DB_FAIL;

	while (!feof(fp))
	{
		fscanf(fp, "%s%s", g_userDB[g_userNum].name, g_userDB[g_userNum].password);
		g_userNum++;
	}

	return g_userNum;
}



Session *addSession(SOCKET sNewConnection)
{
	if (g_online_session_num >= MAX_USER_NUM)
		return NULL;

	Session *session;
	g_online_sessions[g_online_session_num].sock = sNewConnection;
	if (SUCCESS != queue_Create(&g_online_sessions[g_online_session_num].queue))
		return NULL;
	g_online_sessions[g_online_session_num].state = STATE_UNLOGIN;
	session = &g_online_sessions[g_online_session_num];
	g_online_session_num++;
	//if (g_online_session_num > 5)
		//printf("%d  ", g_online_session_num);
	return session;
}

int findSocket(const char* userName, SOCKET* sock)
{
	for (int i = 0; i<g_online_session_num; i++)
	{
		if (strcmp(g_online_sessions[i].userName, userName) == 0
			&& g_online_sessions[i].state == STATE_LOGIN)
		{
			*sock = g_online_sessions[i].sock;
			return FIND_SESSION_SUCCESS;
		}
	}
	return FIND_SESSION_FAIL;
}

int session_queue_Write(SOCKET sock, const char *buff, int length)
{
	for (int i = 0; i<g_online_session_num; i++)
	{
		if (g_online_sessions[i].sock == sock)
		{
			int ret = queue_Write(g_online_sessions[i].queue, buff, length);
			if (ret == SUCCESS)
			{
				return 1;
			}
			else
				return 0;
		}
	}
	return FIND_SESSION_FAIL;
	
}

int removeSession(SOCKET sock)
{
	for (int i = 0; i<g_online_session_num; i++)
	{
		if (g_online_sessions[i].sock == sock)
		{
			queue_Destory(&g_online_sessions[i].queue);
			closesocket(g_online_sessions[i].sock);
			g_online_sessions[i].sock = INVALID_SOCKET;
			for (int j = i; j<g_online_session_num - 1; j++)
			{
				g_online_sessions[j] = g_online_sessions[j + 1];
			}
			g_online_session_num--;
			//if(g_online_session_num > 4)
			//	printf("%d  ", g_online_session_num);
			return REMOVE_SESSION_SUCCESS;
		}
	}
	return UNFOUND_SESSION;
}

int broadcastPkt(PublicChatMsg* publicChatMsg)
{
	publicChatMsg->header.len = htonl(publicChatMsg->header.len);
	publicChatMsg->header.type = htonl(publicChatMsg->header.type);
	for (int i = 0; i<g_online_session_num; i++)
	{
		if (g_online_sessions[i].state == STATE_LOGIN)
			myWSASend(g_online_sessions[i].sock, (const char*)publicChatMsg,
				PUBLIC_CHAT_HEAD + strlen(publicChatMsg->message) + 1, 0);
	}
	return 0;
}

int addFileTable(TransmitFlieCmdPkt transmitFileCmdPkt)
{
	if (WAIT_OBJECT_0 == WaitForSingleObject(g_fileInformation_mutex, INFINITE))
	{
		//文件列表满了
		clearFileTable();
		if (g_fileInformation_num >= MAX_FILE_NUM)
		{
			ReleaseMutex(g_fileInformation_mutex);
			return -FILE_SERVERERROR;
		}

		//添加文件信息
		g_fileInformation[g_fileInformation_num].sockData = INVALID_SOCKET;
		g_fileInformation[g_fileInformation_num].state = STATE_START;
		g_fileInformation[g_fileInformation_num].fileSize = transmitFileCmdPkt.fileSize;
		strncpy(g_fileInformation[g_fileInformation_num].from, transmitFileCmdPkt.from , 20);
		strncpy(g_fileInformation[g_fileInformation_num].to, transmitFileCmdPkt.to, 20);
		ZeroMemory(g_fileInformation[g_fileInformation_num].md5, sizeof(g_fileInformation[g_fileInformation_num].md5));
		strncpy(g_fileInformation[g_fileInformation_num].md5, transmitFileCmdPkt.md5, 32);
		strncpy(g_fileInformation[g_fileInformation_num].filename, transmitFileCmdPkt.filename, 256);
		g_fileInformation_num++;

		ReleaseMutex(g_fileInformation_mutex);
		return 1;
	}
	else
		return -FILE_SERVERERROR;
}

//清理已经传完的
void clearFileTable()
{
	int i;
	for (i = 0; i < g_fileInformation_num - 1; i++)//遍历文件列表
	{
		if (g_fileInformation[i].state == STATE_END)
		{
			//_unlink(g_fileInformation[i].md5);
			for (int j = i; j<g_fileInformation_num - 1; j++)
			{
				g_fileInformation[j] = g_fileInformation[j + 1];
			}
			g_fileInformation_num--;
		}
	}
}

int processFileHeader(SOCKET sock, SOCKET *sockto, FileHeader fileHeader)
{
	*sockto = INVALID_SOCKET;
	if (WAIT_OBJECT_0 == WaitForSingleObject(g_fileInformation_mutex, INFINITE))
	{
		int i;
		if (fileHeader.cmd == FILE_RECV)//接收端连接我了
		{
			for (i = 0; i < g_fileInformation_num; i++)
			{
				if (0 == memcmp(g_fileInformation[i].md5, fileHeader.md5, sizeof(fileHeader.md5))
					&& 0 == strcmp(g_fileInformation[i].to, fileHeader.to))
				{
					g_fileInformation[i].state = STATE_WAITTRANS;
					g_fileInformation[i].fsockto = sock;
					break;
				}
			}
			//创建响应包
			TransmitFlieCmdPkt replyPkt;
			replyPkt.cmd = htonl(FILE_RECV);
			strcpy(replyPkt.filename, g_fileInformation[i].filename);
			replyPkt.fileSize = htonl(g_fileInformation[i].fileSize);
			strcpy(replyPkt.from, g_fileInformation[i].from);
			strcpy(replyPkt.to, g_fileInformation[i].to);
			memcpy(replyPkt.md5, g_fileInformation[i].md5, sizeof(replyPkt.md5));
			replyPkt.header.type = htonl(TRANSMITFILECMD_PKT);
			int len = TRANSMITFILE_HEAD + strlen(replyPkt.filename) + 1;
			replyPkt.header.len = htonl(len - 4);

			SOCKET sockfrom;
			findSocket(g_fileInformation[i].from, &sockfrom);
			myWSASend(sockfrom, (const char*)&replyPkt, len, 0);
		}
		else//发送端连接我了
		{
			for (int i = 0; i < g_fileInformation_num; i++)
			{
				if (0 == memcmp(g_fileInformation[i].md5, fileHeader.md5, sizeof(fileHeader.md5))
					&& 0 == strcmp(g_fileInformation[i].from, fileHeader.from))
				{
					g_fileInformation[i].state = STATE_TRANS;
					g_fileInformation[i].fsockfrom = sock;
					*sockto = g_fileInformation[i].fsockto;
					break;
				}
			}
		}
		ReleaseMutex(g_fileInformation_mutex);
	}
	return 0;
}

int transmitEnd(LPPER_FILE_DATA lpPerFileData)
{
	if (STATE_TRANS == lpPerFileData->state)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(g_fileInformation_mutex, INFINITE))
		{
			int i;
			//找到表中信息并修改状态
			for (i = 0; i < g_fileInformation_num; i++)
			{
				if (0 == strcmp(g_fileInformation[i].md5, lpPerFileData->md5)
					&& g_fileInformation[i].fsockfrom == lpPerFileData->fsockfrom
					&& g_fileInformation[i].fsockto == lpPerFileData->fsockto)
				{
					g_fileInformation[i].state = STATE_END;
					break;
				}
			}

			ReleaseMutex(g_fileInformation_mutex);
		}
	}
	return 0;
}



//方便使用的WSASend
int myWSASend(SOCKET s, const char *buf, int len, DWORD flags)
{
	LPPER_IO_DATA lpPerIOData = (LPPER_IO_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_DATA));
	ZeroMemory(&(lpPerIOData->overlapped), sizeof(OVERLAPPED));
	ZeroMemory(&(lpPerIOData->buffer), sizeof(lpPerIOData->buffer));
	memcpy(lpPerIOData->buffer, buf, len);
	lpPerIOData->bufferLen = len;
	lpPerIOData->databuff.len = len;
	lpPerIOData->databuff.buf = lpPerIOData->buffer;
	lpPerIOData->flags = flags;
	lpPerIOData->operationType = OPERATION_SEND;
	lpPerIOData->socket = s;

	int ret = WSASend(s, &(lpPerIOData->databuff), 1,
		&(lpPerIOData->bufferLen), lpPerIOData->flags, &(lpPerIOData->overlapped), NULL);
	if (-1 == ret&& GetLastError() != ERROR_IO_PENDING)
	{
		MYERROR("WSASend");
		removeSession(s);
		GlobalFree(lpPerIOData);
		lpPerIOData = NULL;
	}
	return 0;
}

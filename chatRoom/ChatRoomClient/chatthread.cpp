#include "chatthread.h"
#include "Protocol.h"

/*
 *
 *
 * qt debug 模式启用多线程一直失败，；；；；；；；
 *
 *
 *
 */

ChatThread::ChatThread()
{
	sClient = 0;
}


void ChatThread:: GetSock(SOCKET client)
{
    sClient = client;
}

void ChatThread::run()
{
	char szRecvBuff[5];
    while(1)
    {
		memset(szRecvBuff, '\0', sizeof(szRecvBuff));
        int a=recv(sClient, szRecvBuff, 4, 0);
        ProcessPkt(szRecvBuff);
    }
}

void ChatThread::ProcessPkt(const char *recvMsg)
{
	int len = ntohl(*(int *)recvMsg);

	char buff[2048];
	memset(buff, '\0', sizeof(buff));
	strcpy(buff, (char *)&recvMsg);

	recv(sClient, buff + 4, len, 0);

    Header* h=(Header*)buff;
    Header header;
    header.length = ntohl(h->length);
    header.type = ntohl(h->type);
    int pktType = header.type;

    if(LOGIN_REPLY_PKT == pktType)
    {
        LoginRelpyPkt* reply = (LoginRelpyPkt*)buff;
		int retcode = ntohl(reply->retCode);
        emit ReciveLoginReply(retcode);
    }
    else if(PUBLIC_CHAT_PKT == pktType)
    {
        PublicChatMsg *publicChatMsg = (PublicChatMsg*)buff;
        QString s1 = publicChatMsg->from;
        QString s2 = publicChatMsg->message;
        emit RecivePublicChat_pkt(s1, s2);
    }
    else if(PRIVATE_CHAT_PKT == pktType)
    { 
        PrivateChatMsg *privateChatMsg = (PrivateChatMsg*)buff;
        QString from = privateChatMsg->from;
		QString to = privateChatMsg->to;
        QString msg = privateChatMsg->message;
        emit RecivePrivateChat_pkt(from, to, msg);
    }
    else if(UPDATE_ONLINEUSER_PKT == pktType)
    {
        ResponsetOnlineUserPkt *userspkt = (ResponsetOnlineUserPkt*)buff;
        int num = htonl(userspkt->OnlineNum);
        QString str = userspkt->users;
        emit ReciveOnlineUser_pkt(num, str);
    }
	else if (WINDOWSSHAKING_PKT == pktType)
	{
		WindowShakingPkt *shakingPkt = (WindowShakingPkt*)buff;
		QString from = shakingPkt->from;
		QString to = shakingPkt->to;
		emit ReciveWindowShaking_pkt(from, to);
	}
    else if(TRANSMITFILECMD_PKT == pktType)
    {
        TransmitFlieCmdPkt *fileReplyPkt = (TransmitFlieCmdPkt*)buff;
        int flag = htonl(fileReplyPkt->cmd);
        QString fileName = fileReplyPkt->filename;
		int fileSize = htonl(fileReplyPkt->fileSize);
		QString from = fileReplyPkt->from;
		QString to = fileReplyPkt->to;
		QString filemd5 = fileReplyPkt->md5;
        emit ReciveFile_Pkt(flag, fileName, from, to, fileSize, filemd5);
    }
    else
    {
        emit DataChanged("unknow: ", "error!");
    }
}


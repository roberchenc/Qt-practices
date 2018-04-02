#ifndef CHATTHREAD_H
#define CHATTHREAD_H

#include <QTime>
#include <QString>
#include <QThread>
#include <WinSock2.h>

/*
*监听线程，用户在登录后，启动该线程，监听与聊天服务器连接的套接字
*收到消息后，首先判断包类型，
*通过信号槽机制转发给主界面
*/


class ChatThread : public QThread
{

    Q_OBJECT
public:
    ChatThread();
    void GetSock(SOCKET client);
    void ProcessPkt(const char *recvMsg);

signals:
    void DataChanged(QString s1, QString s2);
	void ReciveWindowShaking_pkt(QString from, QString to);
    void RecivePublicChat_pkt(QString user, QString msg);
    void RecivePrivateChat_pkt(QString userFrom, QString userTo, QString msg);
    void ReciveLoginReply(int s);
    void ReciveOnlineUser_pkt(int num, QString s2);
    void ReciveFile_Pkt(int flag, QString name, QString from, QString to, int size, QString md5);

    //void ReciveLogout_pkt(QString s1, QString s2);
    //void ReciveLogoutReply_pkt(QString s1, QString s2);

protected:
    void run();

private:
    
    SOCKET sClient;
};

#endif // CHATTHREAD_H

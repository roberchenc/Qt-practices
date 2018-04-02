#ifndef FILETHREAD_H
#define FILETHREAD_H

#include <QThread>
#include <WinSock2.h>
#include <QFile>
#include <qdatastream.h>
#include <QMessageBox>

#include "Protocol.h"

/*
*文件传输线程
*包括发送和接受，每次传输文件操作，都会先开一个线程，保证软件流畅度
*
*/

class FileThread : public QThread
{
    Q_OBJECT

public:
    FileThread();
    FileThread::FileThread(QString file, int fsize);

	void GetSock(SOCKET client);
	void ReciveFile(QString filePath);
	void SendFile();
	void SetRecvFlag(int recv);

signals:
	void SendProgressBarLen(int len);
	void ReciveFileSuccess(int flag);
	void SendFileSuccess(int flag);

protected:
    void run();

private:
    SOCKET dataClient;
    QString filePath;
	int fileSize;

	int ReciveOrSend;

};



#endif // FILETHREAD_H

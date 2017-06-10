#ifndef FILETHREAD_H
#define FILETHREAD_H

#include <QThread>
#include <WinSock2.h>
#include <QFile>
#include <qdatastream.h>
#include <QMessageBox>

#include "Protocol.h"

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

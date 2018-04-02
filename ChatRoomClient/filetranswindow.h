#ifndef FILETRANSWINDOW_H
#define FILETRANSWINDOW_H

#include <WinSock2.h>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

#include <Protocol.h>

#include "filethread.h"
#include "md5.h"

/*
*文件传输窗口在用户点击传文件按钮后弹出，或者在收到接受文件消息后弹出
*传输文件时，先选择文件或者选择文件要放置的位置，
*再连接聊天服务器的文件传输端口，最后通过传文件端口传输文件
*窗口中的进度条通过信号槽与接收/发送线程相连接
*
*/

namespace Ui {
class FileTransWindow;
}

class FileTransWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FileTransWindow(QString ip, QWidget *parent = 0);
    ~FileTransWindow();
	void closeEvent(QCloseEvent *event);

	void GetFileInfo(int size, QString name, QString path, QString filemd5);
	void SetServerIp(QString ip);

private slots:
    void on_btn_choseFile_clicked();

    void on_btn_sendFile_clicked();

	void GetFileOperator(int flag, QString fileName, QString from, QString to, int size, QString filemd5);
	
	void ProgressBarWork(int len);

	void FileOperatorSuccess(int fileFlag);

    //void ReciveSendFileEmit(QString fileName);
signals:
	void FileReciveSuccess(QString fileName, QString from, QString to, int size, QString filemd5);

private:
    Ui::FileTransWindow *ui;

    SOCKET fileClient;
    sockaddr_in fileAddr;

    QString serverIp;
    int serverPort;

    MD5 md5;

    int fileSize;
	int progressBarLen;

    QString filePath;

	QString fileName;
	QString filemd5;
	QString fileFrom;
	QString fileTo;

	FileThread *fThread;

	int sendOrRecv;
	QString fileRecvPath;
};

#endif // FILETRANSWINDOW_H

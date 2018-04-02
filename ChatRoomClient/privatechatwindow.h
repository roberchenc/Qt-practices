#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include <WinSock2.h>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

#include <Protocol.h>

#include "filetranswindow.h"
#include "chatthread.h"
#include "md5.h"
#include "ShakeWindow.h"


/*
*私聊窗口，包含一个历史消息显示框，一个消息输入框，一个抖动按钮，一个传文件按钮
*窗口会在双击在线列表中某个用户时弹出，或者接收到消息后自动弹出
*每个窗口有一个唯一Id保证该私聊窗独立且唯一
*
*/


namespace Ui {
class PrivateChatWindow;
}

class PrivateChatWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PrivateChatWindow(QString ip, SOCKET client, QWidget *parent = 0);
	
    ~PrivateChatWindow();

	void closeEvent(QCloseEvent *event);

	//void SetServerIp(QString ip);

	void SetWindowsId(QString from, QString to);

	QString GetWindowsId();

	void GetPrivateMsg(QString msgFrom, QString msgTo, QString msg);

signals:
	void SendFileInfo(int size, QString name, QString path);
	void closePrivateWindow(QString);
	void StartFileOperator(int flag, QString fileName, QString from, QString to, int size, QString filemd5);
	

private slots:
    void on_btn_shaker_clicked();

    void on_btn_sendMsg_clicked();

    void DisplayPrivateChat_pkt(QString from, QString To, QString msg);

    void on_btn_sendFile_clicked();

	void ProcessFileOperatorPkt(int flag, QString fileName, QString from, QString to, int size, QString filemd5);

	void ReciveFileSuccessProcess(QString fileName, QString from, QString to, int size, QString filemd5);

	void BeginShakingWindow(QString user, QString to);


private:
    Ui::PrivateChatWindow *ui;

	FileTransWindow *fWindow;
	ShakeWindow *shakingWindow;
	
    SOCKET sClient;
	QString serverIp;

	QString chatFrom;
	QString chatTo;

    MD5 md5;
	QString filemd5;

	QString windowsId;
	
    QString fileName;
	QString filePath;
    int fileSize;

};

#endif // PRIVATECHAT_H

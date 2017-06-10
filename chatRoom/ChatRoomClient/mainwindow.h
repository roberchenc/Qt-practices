#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <winsock2.h>
#include <QMessageBox>
#include <QMainWindow>
#include <QTextBrowser>
#include <QTableView>
#include <QStandardItemModel>
#include <QStringListModel>

#include <QString>
#include <Qlist>

#include "chatthread.h"
#include "privatechatwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void UpdateLE();

	void CreatePrivateWindow();

signals:
    void SendUserInfo(SOCKET sock, QString User, QString ToUser);
    
	void PostPrivateMsg(QString msgFrom, QString msgTo, QString msg);

	void PostFileOperator(int flag, QString name, QString from, QString to, int size, QString filemd5);

	void WindowsShaking(QString from, QString to);

	//void StartReciveFile(QString fileName);

private slots:
    void on_btn_login_clicked();

    void on_btn_logout_clicked();

    void on_btn_pushMessage_clicked();

	void on_listv_onlineUser_doubleClicked(const QModelIndex &index);

	void on_btn_updateOnlineUser_clicked();

	void DisplayOnlineUsers(int num, QString users);

    void DisplayPublicChat_pkt(QString from, QString msg);

    void DisplayLoginReply_pkt(int code);

	void ProcessPrivateChat_pkt(QString msgFrom, QString msgTo, QString msg);

	void RemoveUserChatWindow(QString windowsId);

	void ProcessFilePkt(int flag, QString name, QString from, QString to, int size, QString filemd5);

	void ProcessWindowShaking(QString from, QString to);
    

private:
    Ui::MainWindow *ui;

    QStringList onlineUser;
    QStringListModel *model;

    PrivateChatWindow *pwindow;

    WSADATA wsaData;
    sockaddr_in serverAddr;
    SOCKET sClient;

    QString userName, serverIp, password, serverPort, messageContent;

    ChatThread *chatthread;

	FileTransWindow *fWindow;

	QList<QString> chatWindows;
};

#endif // MAINWINDOW_H

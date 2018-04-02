#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <winsock2.h>
#include <QMessageBox>
#include <QMainWindow>
#include <QTextBrowser>
#include <QTableView>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QUrl>
#include <QJsonObject>

#include <QString>
#include <Qlist>

#include "chatthread.h"
#include "privatechatwindow.h"
#include "serversetwindow.h"

/*
*主界面
*程序打开第一个界面，包含菜单栏用于软件设置，状态栏，显示用户状态信息
*包含公共聊天室，即时更新，用户在线列表
*登陆时线连接web服务器，拿到用户信息，比如状态信息，头像信息，
*再连接聊天服务器，聊天功能可以使用了
*
*
*/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    inline void UpdateLE();

	inline void CreatePrivateWindow();

	inline void InitSettings();

	void ShowHeadImage(const QString &szUrl);

signals:
    void SendUserInfo(SOCKET sock, QString User, QString ToUser);
    
	void PostPrivateMsg(QString msgFrom, QString msgTo, QString msg);

	void PostFileOperator(int flag, QString name, QString from, QString to, int size, QString filemd5);

	void WindowsShaking(QString from, QString to);

	//void StartReciveFile(QString fileName);

private slots:
	//有关按钮点击的响应槽函数
    void on_btn_login_clicked();

    void on_btn_logout_clicked();

    void on_btn_pushMessage_clicked();

	void on_btn_updateOnlineUser_clicked();

	void on_listv_onlineUser_doubleClicked(const QModelIndex &index);

	//响应收到某些包之后，将包内容显示在主界面的槽函数
	void DisplayOnlineUsers(int num, QString users);

    void DisplayPublicChat_pkt(QString from, QString msg);

    void DisplayLoginReply_pkt(int code);

	//处理私聊包
	void ProcessPrivateChat_pkt(QString msgFrom, QString msgTo, QString msg);

	void RemoveUserChatWindow(QString windowsId);

	//处理文件传输包
	void ProcessFilePkt(int flag, QString name, QString from, QString to, int size, QString filemd5);

	//处理窗口抖动包
	void ProcessWindowShaking(QString from, QString to);

    void SetServer(QString ip, QString port);

    void OpenServerSet();

    void replyFinished(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;
	QLabel *userState;

    QNetworkAccessManager *netManager;

    QStringList onlineUser;
    QStringListModel *model;

    PrivateChatWindow *pwindow;
    ServerSetWindow *swindow;

    WSADATA wsaData;
    sockaddr_in serverAddr;
    SOCKET sClient;

    QString userName, serverIp, password, serverPort, messageContent;
	QString imageUrl;
	QString stateInfo;

    ChatThread *chatthread;

	FileTransWindow *fWindow;

	QList<QString> chatWindows;
};

#endif // MAINWINDOW_H

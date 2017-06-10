#include <string>

#include "chatthread.h"
#include "Protocol.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    chatthread = new ChatThread;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData)!=0)
    {
        QMessageBox::information(NULL, "error", "WSAStartup failed!",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }

    ui->btn_logout->setEnabled(false);
    ui->le_server->setText("192.168.63.158");
    ui->le_port->setText("8888");
    ui->le_name->setText("aaa");
    ui->le_psw->setText("123");
    
    memset(&serverAddr, 0, sizeof(serverAddr));

    model = new QStringListModel(onlineUser);
    ui->listv_onlineUser->setModel(model);        //useList是个QListView

    onlineUser += "first";
    onlineUser += "second";
    model->setStringList(onlineUser);

	UpdateLE();

    //绑定信号槽
    connect(chatthread, SIGNAL(RecivePublicChat_pkt(QString, QString)),
            this, SLOT(DisplayPublicChat_pkt(QString, QString)));

    connect(chatthread, SIGNAL(ReciveLoginReply(int)),
            this, SLOT(DisplayLoginReply_pkt(int)));

    connect(chatthread, SIGNAL(ReciveOnlineUser_pkt(int, QString)),
            this, SLOT(DisplayOnlineUsers(int, QString)));

	connect(chatthread, SIGNAL(RecivePrivateChat_pkt(QString, QString, QString)),
		this, SLOT(ProcessPrivateChat_pkt(QString, QString, QString)));

	connect(chatthread, SIGNAL(ReciveFile_Pkt(int, QString, QString, QString, int, QString)),
		this, SLOT(ProcessFilePkt(int, QString, QString, QString, int, QString)));

     connect(chatthread, SIGNAL(ReciveWindowShaking_pkt(QString, QString)),
            this, SLOT(ProcessWindowShaking(QString, QString)));
}

MainWindow::~MainWindow()
{
    if(WSACleanup()==SOCKET_ERROR)
        QMessageBox::information(NULL, "error", "WSACleanup failed!",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    delete ui;
}

void MainWindow::RemoveUserChatWindow(QString windowsId)
{
	chatWindows.removeOne(windowsId);
}

void MainWindow::ProcessWindowShaking(QString from, QString to)
{
	QString id = to + from;
	if (chatWindows.indexOf(id) == -1)
	{
		CreatePrivateWindow();
		pwindow->SetWindowsId(to, from);
		//pwindow->SetServerIp(serverIp);
		//pwindow->GetUserInfo(sClient, to, from);

		chatWindows.append(pwindow->GetWindowsId());
	}
	pwindow->show();
	emit WindowsShaking(from, to); 
}

void MainWindow::ProcessFilePkt(int flag, QString name, QString from, QString to, int size, QString filemd5)
{
	QString s = "";
	if (flag == FILE_RECV)
	{
		s = from+to;
	}
	else if (flag == FILE_SEND)
	{
		s = to + from;
	}
	else if (FILE_RECVSUCCESS == flag)
	{
		QMessageBox::information(NULL, "Success", "File Recive Success!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	else if (FILE_EXIST == flag)
	{
		QMessageBox::information(NULL, "Success", "File has Sent Success!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		s = from + to;
	}
	else if (FILE_SERVERERROR == flag)
	{
		QMessageBox::information(NULL, "Error", "Server Error!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	else if (FILE_NOTFOUND == flag)
	{
		QMessageBox::information(NULL, "Error", "File Not Found!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	
	if (chatWindows.indexOf(s) == -1)
	{
		CreatePrivateWindow();
		pwindow->SetWindowsId(to, from);
		//pwindow->SetServerIp(serverIp);
		//pwindow->GetUserInfo(sClient, to, from);

		chatWindows.append(pwindow->GetWindowsId());
	}
	pwindow->show();
	emit PostFileOperator(flag, name, from, to, size, filemd5);
}

void MainWindow::ProcessPrivateChat_pkt(QString msgFrom, QString msgTo, QString msg)
{
	QString s = msgTo + msgFrom;
	if (chatWindows.indexOf(s) == -1)
	{
		CreatePrivateWindow();
		pwindow->SetWindowsId(msgTo, msgFrom);
		//pwindow->SetServerIp(serverIp);
		//pwindow->GetUserInfo(sClient, msgTo, msgFrom);

		chatWindows.append(pwindow->GetWindowsId());
	}
	pwindow->show();
	emit PostPrivateMsg(msgFrom, msgTo, msg);
}


void MainWindow::DisplayOnlineUsers(int num, QString users)
{
    int i=0;
    QString temp;
	onlineUser.clear();
    while(i <= users.length())
    {
        if(QChar('\n') != users[i])
            temp+=users[i];
		else
		{
			onlineUser += temp;
			model->setStringList(onlineUser);
			temp = "";
		}
        
        i++;
    }
}

void MainWindow::DisplayPublicChat_pkt(QString from, QString msg)
{
	from += " : ";
	ui->tb_publicMessage->append(from + msg);
}

void MainWindow::DisplayLoginReply_pkt(int code)
{
	if (LOGIN_SUCCESS == code)
	{
		QMessageBox::information(NULL, userName, "Login Success!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		ui->btn_login->setEnabled(false);
		ui->btn_logout->setEnabled(true);
		QString s = "System : ";
		s += userName;
		s += " has logined!";
		ui->tb_publicMessage->append(s);

		onlineUser += userName;
		model->setStringList(onlineUser);

	}
	else
	{
		chatthread->exit();
		closesocket(sClient);
		sClient = INVALID_SOCKET;
		QMessageBox::information(NULL, userName, "Login failed(has login..)!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}

}

void MainWindow::on_btn_login_clicked()
{
	sClient = socket(PF_INET, SOCK_STREAM, 0);
	if (sClient == INVALID_SOCKET)
	{
		QMessageBox::information(NULL, "error", "socket create failed!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	}
    UpdateLE();
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIp.toStdString().data());
    serverAddr.sin_port = htons(serverPort.toInt());

    if(::connect(sClient, (sockaddr*)&serverAddr, sizeof(serverAddr))==SOCKET_ERROR)
        QMessageBox::information(NULL, "error", "connect server failed!",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    LoginPkt pkt;
    memset(&pkt, '\0', sizeof(LoginPkt));
    pkt.header.type=htonl(LOGIN_PKT);

    strcpy(pkt.password,password.toStdString().data());
    strcpy(pkt.userName, userName.toStdString().data());
    pkt.header.length=htonl(sizeof(pkt)-4);
    //向服务端发送tcp消息
    if(send(sClient, (const char*)&pkt, sizeof(pkt), 0)<0)
		QMessageBox::information(NULL, "error", "send pkt failed!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    chatthread->GetSock(sClient);
    chatthread->start();
}

void MainWindow::on_btn_logout_clicked()
{
    chatthread->exit();

    LogoutPkt pkt;
    pkt.header.type=htonl(LOGOUT_PKT);

    strcpy(pkt.userName,userName.toStdString().data());
    pkt.header.length = htonl(sizeof(pkt)-4);
    if(send(sClient, (const char*)&pkt, sizeof(pkt), 0)<0)
		QMessageBox::information(NULL, "error", "send pkt failed!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    QString s("System : ");
    s += pkt.userName;
    QString s1("  logout!");
    ui->tb_publicMessage->append(s+s1);

	ui->btn_login->setEnabled(true);
	ui->btn_logout->setEnabled(false);
	
	closesocket(sClient);

	int i = onlineUser.indexOf(userName);
	onlineUser.removeAt(i);
	QModelIndex index = model->index(i);
	model->removeRows(i, 1);
	QMessageBox::information(NULL, userName, "Logout Success!",
		QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

void MainWindow::on_btn_pushMessage_clicked()
{
	messageContent = ui->le_message->text();

    PublicChatMsg pkt;
    memset((char *)&pkt, '\0', sizeof(pkt));
    pkt.header.type=htonl(PUBLIC_CHAT_PKT);
    strcpy(pkt.message,messageContent.toStdString().data());
    strcpy(pkt.from, userName.toStdString().data());
    pkt.header.length = htonl(sizeof(pkt.from) + strlen(pkt.message) + 1 + sizeof(int));

	int len = ntohl(pkt.header.length);

    if(send(sClient, (const char*)&pkt, len+4, 0)<0)
		QMessageBox::information(NULL, "error", "send pkt failed!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	//int error = GetLastError();
	//printf("");
}

void MainWindow::on_listv_onlineUser_doubleClicked(const QModelIndex &index)
{
    QString chatTo = index.data().toString();
	QString s = userName + chatTo;

	if (chatWindows.indexOf(s) == -1)
	{
		CreatePrivateWindow();
		pwindow->SetWindowsId(userName, chatTo);
		//pwindow->SetServerIp(serverIp);
		//pwindow->GetUserInfo(sClient, userName, chatTo);
		chatWindows.append(s);
	}
	pwindow->show();
}

void MainWindow::on_btn_updateOnlineUser_clicked()
{
    RequestOnlineUserPkt pkt;
    memset((char *)&pkt, '\0', sizeof(pkt));
	
    pkt.header.type=htonl(UPDATE_ONLINEUSER_PKT);
    pkt.header.length=htonl(sizeof(pkt)-sizeof(int));
	pkt.ReqNumber = 10;
	if(send(sClient, (const char*)&pkt, sizeof(pkt), 0)<0)
		QMessageBox::information(NULL, "error", "send pkt failed!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

void MainWindow::UpdateLE()
{
	userName = ui->le_name->text();
	serverIp = ui->le_server->text();
	password = ui->le_psw->text();
	serverPort = ui->le_port->text();
}

void MainWindow::CreatePrivateWindow()
{
	pwindow = new PrivateChatWindow(serverIp, sClient);

	connect(this, SIGNAL(PostPrivateMsg(QString, QString, QString)),
		pwindow, SLOT(DisplayPrivateChat_pkt(QString, QString, QString)));
	connect(pwindow, SIGNAL(closePrivateWindow(QString)),
		this, SLOT(RemoveUserChatWindow(QString)));
	connect(this, SIGNAL(PostFileOperator(int, QString, QString, QString, int, QString)),
		pwindow, SLOT(ProcessFileOperatorPkt(int, QString, QString, QString, int, QString)));
	connect(this, SIGNAL(WindowsShaking(QString, QString)),
		pwindow, SLOT(BeginShakingWindow(QString, QString)));
}



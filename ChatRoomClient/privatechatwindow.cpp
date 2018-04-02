#include "privatechatwindow.h"
#include "ui_privatechatwindow.h"
#include <iostream>
#include <io.h>

PrivateChatWindow::PrivateChatWindow(QString ip, SOCKET client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrivateChatWindow)
{
    ui->setupUi(this);
	shakingWindow = new ShakeWindow(this);
	serverIp = ip;
	sClient = client;

	chatFrom = chatTo = "";
	windowsId = "";
	fileName = "";
}

PrivateChatWindow::~PrivateChatWindow()
{
    delete ui;
}

//void PrivateChatWindow::SetServerIp(QString ip)
//{
//	serverIp = ip;
//}

void PrivateChatWindow::SetWindowsId(QString from, QString to)
{
	windowsId = from + to;
	chatFrom = from;
	chatTo = to;

	ui->tb_historyMsg->append("System : " + from + " chat with " + to);
}

QString PrivateChatWindow::GetWindowsId()
{
	return windowsId;
}

void PrivateChatWindow::GetPrivateMsg(QString msgFrom, QString msgTo, QString msg)
{
	QString s = msgTo + msgFrom;
	if (s == windowsId)
	{
		msgFrom += " : ";
		QString s = msgFrom + msg;
		ui->tb_historyMsg->append(s);
	}
}

void PrivateChatWindow::closeEvent(QCloseEvent *event)
{
	emit closePrivateWindow(windowsId);
}

void PrivateChatWindow::ReciveFileSuccessProcess(QString fileName, QString from, QString to, int size, QString filemd5)
{
	QString id = to + from;
	if (id != windowsId)
	{
		return;
	}
	TransmitFlieCmdPkt pkt;
	
	strcpy(pkt.md5, filemd5.toStdString().c_str());

	pkt.header.type = htonl(TRANSMITFILECMD_PKT);
	pkt.cmd = htonl(FILE_RECVSUCCESS);
	strcpy(pkt.from, from.toStdString().c_str());
	strcpy(pkt.to, to.toStdString().c_str());
	strcpy(pkt.filename, fileName.toStdString().c_str());

	int len = sizeof(pkt) - 259 + strlen(pkt.filename);
	pkt.header.length = htonl(len);
	pkt.fileSize = htonl(fileSize);

	if (send(sClient, (const char*)&pkt, len + 4, 0) < 0)
		QMessageBox::information(NULL, "error", "send pkt failed!",
						QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

void PrivateChatWindow::BeginShakingWindow(QString from, QString to)
{
	QString s = to + from;
	if (s == windowsId)
	{
		shakingWindow->startShake();
	}
}

void PrivateChatWindow::ProcessFileOperatorPkt(int flag, QString fileName, QString from, QString to, int size, QString filemd5)
{
	QString id = "";
	if (FILE_RECVSUCCESS == flag)
	{
		QMessageBox::information(NULL, "Success", "File Recive Success!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	else if (FILE_EXIST == flag)
	{
		/*QMessageBox::information(NULL, "Success", "File has Sent Success!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);*/
		fWindow->close();
		return;
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
	else if (FILE_RECV == flag)
		id = from + to;
	else if (FILE_SEND == flag)
		id = to + from;
	if (id != windowsId)
	{
		return;
	}
	if (FILE_RECV == flag)
	{
		emit StartFileOperator(flag, fileName, from, to, size, filemd5);
	}
	else if (FILE_SEND == flag)
	{
		QMessageBox::information(NULL, "Info", "有新文件等待接受!",
						QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		fWindow = new FileTransWindow(serverIp);
		connect(this, SIGNAL(StartFileOperator(int, QString, QString, QString, int, QString)),
			fWindow, SLOT(GetFileOperator(int, QString, QString, QString, int, QString)));
		connect(fWindow, SIGNAL(FileReciveSuccess(QString, QString, QString, int, QString)),
			this, SLOT(ReciveFileSuccessProcess(QString, QString, QString, int, QString)));

		//fWindow->SetServerIp(serverIp);
		fWindow->show();
		emit StartFileOperator(flag, fileName, from, to, size, filemd5);
		return;
	}
	
}


void PrivateChatWindow::on_btn_sendFile_clicked()
{
	if (chatFrom == chatTo)
	{
		QMessageBox::information(NULL, "Info", "No need send file to yourself!(foolish)",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	filePath = QFileDialog::getOpenFileName(this, tr("open file"), " ", tr("Allfile(*.*);;mp3file(*.mp3)"));
	if (filePath == "")
	{
		QMessageBox::information(NULL, "Info", "please 选择文件!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	int first = filePath.lastIndexOf("/"); //从后面查找"/"位置
	fileName = filePath.right(filePath.length() - first - 1); //从右边截取
	FILE* file = fopen(filePath.toStdString().c_str(), "r");
	if (file)
	{
		fileSize = filelength(fileno(file));
		fclose(file);
	}
	if (fileSize == 0)
	{
		QMessageBox::information(NULL, "Info", "Can't Send NULL File!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}

	//发送开始发文件的第一个包--tranmitfilepkt
	TransmitFlieCmdPkt pkt;
	md5.reset();
	md5.update(ifstream(filePath.toStdString().c_str()));
	strcpy(pkt.md5, md5.toString().c_str());

	pkt.header.type = htonl(TRANSMITFILECMD_PKT);
	pkt.cmd = htonl(FILE_SEND);
	strcpy(pkt.from, chatFrom.toStdString().c_str());
	strcpy(pkt.to, chatTo.toStdString().c_str());
	strcpy(pkt.filename, fileName.toStdString().c_str());

	int len = sizeof(pkt) - 259 + strlen(pkt.filename);
	pkt.header.length = htonl(len);
	pkt.fileSize = htonl(fileSize);

	if (send(sClient, (const char*)&pkt, len + 4, 0) < 0)
		QMessageBox::information(NULL, "error", "send pkt failed!",
						QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	//生成一个文件发送窗口，并向这个窗口传文件相关信息
	fWindow = new FileTransWindow(serverIp);
	connect(this, SIGNAL(StartFileOperator(int, QString, QString, QString, int, QString)),
		fWindow, SLOT(GetFileOperator(int, QString, QString, QString, int, QString)));
	connect(fWindow, SIGNAL(FileReciveSuccess(QString, QString, QString, int, QString)),
		this, SLOT(ReciveFileSuccessProcess(QString, QString, QString, int, QString)));

	//fWindow->SetServerIp(serverIp);
	fWindow->GetFileInfo(fileSize, fileName, filePath, filemd5);
	fWindow->show();	
}

void PrivateChatWindow::on_btn_shaker_clicked()
{
	WindowShakingPkt pkt;
	pkt.header.type = htonl(WINDOWSSHAKING_PKT);
	
	strcpy(pkt.from, chatFrom.toStdString().c_str());
	strcpy(pkt.to, chatTo.toStdString().c_str());
	int len = sizeof(pkt)-4;
	pkt.header.length = htonl(len);
	if (send(sClient, (const char*)&pkt, len + 4, 0)<0)
		QMessageBox::information(NULL, "error", "send pkt failed!",
						QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	shakingWindow->startShake();
}

void PrivateChatWindow::on_btn_sendMsg_clicked()
{
    QString message = ui->le_inputMsg->text();

    PrivateChatMsg pkt;
    memset((char *)&pkt, '\0', sizeof(pkt));

    pkt.header.type=htonl(PRIVATE_CHAT_PKT);
    strcpy(pkt.message,message.toStdString().data());
    strcpy(pkt.from, chatFrom.toStdString().data());
    strcpy(pkt.to, chatTo.toStdString().data());
	int len = sizeof(pkt.to) + sizeof(pkt.from) + strlen(pkt.message) + sizeof(int) + 1;
    pkt.header.length = htonl(len);

    QString str=chatFrom + ": ";
    //QString s2 = ui->le_inputMsg->text();
    ui->tb_historyMsg->append(str+message);

    if(send(sClient, (const char*)&pkt, len+4, 0)<0)
		QMessageBox::information(NULL, "error", "send pkt failed!",
						QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

void PrivateChatWindow::DisplayPrivateChat_pkt(QString msgFrom, QString msgTo, QString msg)
{
	QString s = msgTo + msgFrom;
	if (s == windowsId)
	{
		msgFrom += " : ";
		QString s = msgFrom + msg;
		ui->tb_historyMsg->append(s);
	}
}




#include "filetranswindow.h"
#include "ui_filetranswindow.h"
#include <io.h>


FileTransWindow::FileTransWindow(QString ip, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileTransWindow)
{
    ui->setupUi(this);
	serverIp = ip;
	serverPort = 8889;
	progressBarLen = 0;
    ui->Bar_progressFile->setVisible(false);
    ui->label_fileState->setVisible(false);
	ui->btn_choseFile->setEnabled(false);
	ui->btn_sendFile->setEnabled(false);
	sendOrRecv = 0;
}

FileTransWindow::~FileTransWindow()
{
    delete ui;
}

void FileTransWindow::closeEvent(QCloseEvent *event)
{
	closesocket(fileClient);
}

void FileTransWindow::SetServerIp(QString ip)
{
	serverIp = ip;
}

void FileTransWindow::ProgressBarWork(int len)
{
	progressBarLen += len;
	ui->Bar_progressFile->setValue(progressBarLen);
	
}

void FileTransWindow::GetFileInfo(int size, QString name, QString path, QString md5)
{
	fileSize = size;
	fileName = name;
	filePath = path;
	filemd5 = md5;
	ui->label_filePath->setText(path);
	
}

void FileTransWindow::FileOperatorSuccess(int fileFlag)
{
	if (FILE_RECV == fileFlag)
	{
		ui->btn_choseFile->setText("Success");
		ui->btn_choseFile->setEnabled(false);
		emit FileReciveSuccess(fileName, fileFrom, fileTo, fileSize, filemd5);
		closesocket(fileClient);
		this->close();
	}
	if (FILE_SEND == fileFlag)
	{
		ui->btn_sendFile->setText("Success");
		ui->btn_sendFile->setEnabled(false);
		closesocket(fileClient);
		this->close();
	}
}

void FileTransWindow::GetFileOperator(int flag, QString name, QString from, QString to, int size, QString md5)
{
	sendOrRecv = flag;
	fileName = name;
	ui->btn_choseFile->setEnabled(true);
	ui->label_fileState->setVisible(true);
	ui->btn_sendFile->setEnabled(false);
	if (FILE_RECV == flag)
	{
		if (name == fileName)
		{
			ui->btn_choseFile->setText(tr("Connect"));
			ui->label_fileState->setText("请连接服务器.....");
			fileFrom = from;
			fileTo = to;
			fileSize = size;
			filemd5 = md5;
		}
	}
	else if (FILE_SEND == flag)
	{
		ui->label_filePath->setText("D:/" + name);
		ui->btn_choseFile->setText("Recive");
		ui->label_fileState->setText(tr("Ready To Recive..."));
		ui->Bar_progressFile->setMaximum(size);
		filemd5 = md5;
		fileFrom = from;
		fileTo = to;
		fileSize = size;
	}
}

void FileTransWindow::on_btn_choseFile_clicked()
{
	if (sendOrRecv != FILE_RECV)
	{
		fileRecvPath = QFileDialog::getExistingDirectory(this, "请选择文件保存路径...", "./");
		if (fileRecvPath.isEmpty())
		{
			QMessageBox::information(NULL, "Info", "No Selected! Please Again.",
				QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return;
		}
		fileRecvPath += "/";
		fileRecvPath += fileName;
	}
	
    //使用新创建的数据端口(socket),开始向服务器发送文件
    fileClient = socket(PF_INET, SOCK_STREAM, 0);
    if (fileClient == INVALID_SOCKET)
    {
        QMessageBox::information(NULL, "error", "socket create failed!",
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
    }
    fileAddr.sin_family = AF_INET;
    fileAddr.sin_addr.s_addr = inet_addr(serverIp.toStdString().data());
    fileAddr.sin_port = htons(serverPort);

	if (::connect(fileClient, (sockaddr*)&fileAddr, sizeof(fileAddr)) == SOCKET_ERROR)
	{
		QMessageBox::information(NULL, "error", "connect server failed!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}

    FileHeader fHeader;
	if(sendOrRecv != FILE_RECV)
		fHeader.cmd = htonl(FILE_RECV);
	else
		fHeader.cmd=htonl(FILE_SEND);

    fHeader.fileSize=htonl(fileSize);
	
	strcpy(fHeader.from, fileFrom.toStdString().c_str());
    strcpy(fHeader.to, fileTo.toStdString().c_str());
	strcpy(fHeader.md5, filemd5.toStdString().c_str());

	if (send(fileClient, (const char*)&fHeader, sizeof(fHeader), 0) < 0)
	{
		QMessageBox::information(NULL, "error", "send pkt failed!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	
    //至此传输文件的准备工作完成，接下来开启一个新的线程传送文件，传输结束，线程关闭
	if (sendOrRecv != FILE_RECV)
	{
		ui->btn_sendFile->setEnabled(false);
		ui->btn_choseFile->setText("Reciving...");
		ui->label_fileState->setText("Reciving File...");
		if (fileRecvPath == "")
			return;

		fThread = new FileThread(fileRecvPath, fileSize);
		connect(fThread, SIGNAL(ReciveFileSuccess(int)),
			this, SLOT(FileOperatorSuccess(int)));
		connect(fThread, SIGNAL(SendProgressBarLen(int)),
			this, SLOT(ProgressBarWork(int)));

		ui->Bar_progressFile->setVisible(true);
		ui->Bar_progressFile->setValue(0);
		progressBarLen = 0;
		fThread->GetSock(fileClient);
		fThread->SetRecvFlag(FILE_RECV);
		fThread->start();
	}
	else
	{
		ui->btn_choseFile->setText("Connected");
		ui->btn_sendFile->setEnabled(true);
		ui->label_fileState->setText("Ready To Send File...");
	}
	ui->btn_choseFile->setEnabled(false);
}

void FileTransWindow::on_btn_sendFile_clicked()
{
    ui->Bar_progressFile->setVisible(true);
	ui->Bar_progressFile->setMaximum(fileSize);
    ui->Bar_progressFile->setValue(0);
    ui->label_fileState->setVisible(true);
	ui->label_fileState->setText("Sending File...");
	fThread = new FileThread(filePath, fileSize);
	connect(fThread, SIGNAL(SendProgressBarLen(int)),
		this, SLOT(ProgressBarWork(int)));
	connect(fThread, SIGNAL(SendFileSuccess(int)),
		this, SLOT(FileOperatorSuccess(int)));

	fThread->GetSock(fileClient);
	fThread->start();
}

#include "serversetwindow.h"
#include "ui_serversetwindow.h"

ServerSetWindow::ServerSetWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerSetWindow)
{
    ui->setupUi(this);
    ui->lineEdit_ip->setText("115.159.200.151");
    ui->lineEdit_port->setText("8888");
    serverIp="127.0.0.1";
    serverPort="8888";
}

ServerSetWindow::~ServerSetWindow()
{
    delete ui;
}

void ServerSetWindow::on_btn_ok_clicked()
{
    serverIp=ui->lineEdit_ip->text();
    serverPort=ui->lineEdit_port->text();
    emit serverSet(serverIp, serverPort);
    QSettings *configIniWrite = new QSettings("chatroom.ini", QSettings::IniFormat);
	configIniWrite->setIniCodec("UTF-8");
   //向ini文件中写入内容,setValue函数的两个参数是键值对
   //向ini文件的第一个节写入内容,ip节下的第一个参数
	configIniWrite->beginGroup("server");
   configIniWrite->setValue("ip", serverIp);
   //向ini文件的第一个节写入内容,ip节下的第二个参数
   configIniWrite->setValue("port", serverPort);
   configIniWrite->endGroup();
   delete configIniWrite;
    this->hide();
}

void ServerSetWindow::on_btn_cancle_clicked()
{
    //serverIp="127.0.0.1";
    //serverPort="8888";
    this->hide();
}

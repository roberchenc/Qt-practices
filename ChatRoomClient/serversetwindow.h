#ifndef SERVERSET_H
#define SERVERSET_H

#include <QDialog>
#include <QString>
#include <QSettings>

namespace Ui {
class ServerSetWindow;
}

/*
*服务器设置窗口，用来修改服务器的ip地址和端口号，
*并保存为ini配置文件
*
*/
class ServerSetWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ServerSetWindow(QWidget *parent = 0);
    ~ServerSetWindow();

signals:
    void serverSet(QString serverIp, QString serverPort);

private slots:
    void on_btn_ok_clicked();

    void on_btn_cancle_clicked();

private:
    Ui::ServerSetWindow *ui;

    QString serverIp, serverPort;

};

#endif // SERVERSET_H

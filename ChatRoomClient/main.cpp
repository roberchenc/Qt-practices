#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <QApplication>

#include "mainwindow.h"
#include "privatechatwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}

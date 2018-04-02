/********************************************************************************
** Form generated from reading UI file 'serversetwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERSETWINDOW_H
#define UI_SERVERSETWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ServerSetWindow
{
public:
    QLineEdit *lineEdit_ip;
    QLineEdit *lineEdit_port;
    QLabel *label_ip;
    QLabel *label_port;
    QPushButton *btn_cancle;
    QPushButton *btn_ok;

    void setupUi(QDialog *ServerSetWindow)
    {
        if (ServerSetWindow->objectName().isEmpty())
            ServerSetWindow->setObjectName(QStringLiteral("ServerSetWindow"));
        ServerSetWindow->resize(400, 300);
        lineEdit_ip = new QLineEdit(ServerSetWindow);
        lineEdit_ip->setObjectName(QStringLiteral("lineEdit_ip"));
        lineEdit_ip->setGeometry(QRect(140, 40, 161, 20));
        lineEdit_port = new QLineEdit(ServerSetWindow);
        lineEdit_port->setObjectName(QStringLiteral("lineEdit_port"));
        lineEdit_port->setGeometry(QRect(140, 130, 161, 20));
        label_ip = new QLabel(ServerSetWindow);
        label_ip->setObjectName(QStringLiteral("label_ip"));
        label_ip->setGeometry(QRect(40, 40, 54, 12));
        label_port = new QLabel(ServerSetWindow);
        label_port->setObjectName(QStringLiteral("label_port"));
        label_port->setGeometry(QRect(40, 130, 54, 12));
        btn_cancle = new QPushButton(ServerSetWindow);
        btn_cancle->setObjectName(QStringLiteral("btn_cancle"));
        btn_cancle->setGeometry(QRect(70, 230, 75, 23));
        btn_ok = new QPushButton(ServerSetWindow);
        btn_ok->setObjectName(QStringLiteral("btn_ok"));
        btn_ok->setGeometry(QRect(240, 230, 75, 23));

        retranslateUi(ServerSetWindow);

        QMetaObject::connectSlotsByName(ServerSetWindow);
    } // setupUi

    void retranslateUi(QDialog *ServerSetWindow)
    {
        ServerSetWindow->setWindowTitle(QApplication::translate("ServerSetWindow", "ServerSet", Q_NULLPTR));
        label_ip->setText(QApplication::translate("ServerSetWindow", "serverIp", Q_NULLPTR));
        label_port->setText(QApplication::translate("ServerSetWindow", "serverPort", Q_NULLPTR));
        btn_cancle->setText(QApplication::translate("ServerSetWindow", "Cancel", Q_NULLPTR));
        btn_ok->setText(QApplication::translate("ServerSetWindow", "Ok", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ServerSetWindow: public Ui_ServerSetWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERSETWINDOW_H

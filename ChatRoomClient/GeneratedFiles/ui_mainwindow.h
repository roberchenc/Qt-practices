/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *btn_login;
    QPushButton *btn_logout;
    QLabel *label_name;
    QLabel *label_psw;
    QLineEdit *le_name;
    QLineEdit *le_psw;
    QLabel *label_message;
    QLineEdit *le_message;
    QPushButton *btn_pushMessage;
    QLabel *label_online;
    QTextBrowser *tb_publicMessage;
    QListView *listv_onlineUser;
    QPushButton *btn_updateOnlineUser;
    QLabel *label_headImage;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1016, 642);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        btn_login = new QPushButton(centralWidget);
        btn_login->setObjectName(QStringLiteral("btn_login"));
        btn_login->setGeometry(QRect(610, 30, 81, 31));
        btn_logout = new QPushButton(centralWidget);
        btn_logout->setObjectName(QStringLiteral("btn_logout"));
        btn_logout->setGeometry(QRect(610, 100, 81, 31));
        label_name = new QLabel(centralWidget);
        label_name->setObjectName(QStringLiteral("label_name"));
        label_name->setGeometry(QRect(330, 30, 54, 20));
        label_psw = new QLabel(centralWidget);
        label_psw->setObjectName(QStringLiteral("label_psw"));
        label_psw->setGeometry(QRect(330, 110, 54, 12));
        le_name = new QLineEdit(centralWidget);
        le_name->setObjectName(QStringLiteral("le_name"));
        le_name->setGeometry(QRect(380, 30, 191, 20));
        le_psw = new QLineEdit(centralWidget);
        le_psw->setObjectName(QStringLiteral("le_psw"));
        le_psw->setGeometry(QRect(380, 110, 191, 20));
        le_psw->setEchoMode(QLineEdit::Password);
        label_message = new QLabel(centralWidget);
        label_message->setObjectName(QStringLiteral("label_message"));
        label_message->setGeometry(QRect(50, 170, 91, 16));
        le_message = new QLineEdit(centralWidget);
        le_message->setObjectName(QStringLiteral("le_message"));
        le_message->setGeometry(QRect(50, 530, 421, 31));
        btn_pushMessage = new QPushButton(centralWidget);
        btn_pushMessage->setObjectName(QStringLiteral("btn_pushMessage"));
        btn_pushMessage->setGeometry(QRect(500, 520, 111, 41));
        label_online = new QLabel(centralWidget);
        label_online->setObjectName(QStringLiteral("label_online"));
        label_online->setGeometry(QRect(750, 20, 54, 12));
        tb_publicMessage = new QTextBrowser(centralWidget);
        tb_publicMessage->setObjectName(QStringLiteral("tb_publicMessage"));
        tb_publicMessage->setGeometry(QRect(50, 200, 551, 301));
        listv_onlineUser = new QListView(centralWidget);
        listv_onlineUser->setObjectName(QStringLiteral("listv_onlineUser"));
        listv_onlineUser->setGeometry(QRect(740, 50, 256, 511));
        btn_updateOnlineUser = new QPushButton(centralWidget);
        btn_updateOnlineUser->setObjectName(QStringLiteral("btn_updateOnlineUser"));
        btn_updateOnlineUser->setGeometry(QRect(910, 10, 61, 31));
        label_headImage = new QLabel(centralWidget);
        label_headImage->setObjectName(QStringLiteral("label_headImage"));
        label_headImage->setGeometry(QRect(90, 20, 141, 131));
        label_headImage->setMouseTracking(true);
        label_headImage->setAutoFillBackground(false);
        label_headImage->setScaledContents(true);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1016, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "ChatRoom", Q_NULLPTR));
        btn_login->setText(QApplication::translate("MainWindow", "\347\231\273\345\275\225", Q_NULLPTR));
        btn_logout->setText(QApplication::translate("MainWindow", "\347\231\273\345\207\272", Q_NULLPTR));
        label_name->setText(QApplication::translate("MainWindow", "\347\224\250\346\210\267\345\220\215", Q_NULLPTR));
        label_psw->setText(QApplication::translate("MainWindow", "\345\257\206\347\240\201", Q_NULLPTR));
        label_message->setText(QApplication::translate("MainWindow", "\350\201\212\345\244\251\345\256\244\346\266\210\346\201\257\345\216\206\345\217\262", Q_NULLPTR));
        btn_pushMessage->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201\346\266\210\346\201\257", Q_NULLPTR));
        label_online->setText(QApplication::translate("MainWindow", "\345\234\250\347\272\277\345\210\227\350\241\250", Q_NULLPTR));
        btn_updateOnlineUser->setText(QApplication::translate("MainWindow", "Update", Q_NULLPTR));
        label_headImage->setText(QApplication::translate("MainWindow", "\345\244\264\345\203\217", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

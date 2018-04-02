/********************************************************************************
** Form generated from reading UI file 'privatechatwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRIVATECHATWINDOW_H
#define UI_PRIVATECHATWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_PrivateChatWindow
{
public:
    QPushButton *btn_sendFile;
    QPushButton *btn_shaker;
    QPushButton *btn_sendMsg;
    QLabel *label_historyMsg;
    QTextBrowser *tb_historyMsg;
    QLineEdit *le_inputMsg;

    void setupUi(QDialog *PrivateChatWindow)
    {
        if (PrivateChatWindow->objectName().isEmpty())
            PrivateChatWindow->setObjectName(QStringLiteral("PrivateChatWindow"));
        PrivateChatWindow->resize(578, 486);
        btn_sendFile = new QPushButton(PrivateChatWindow);
        btn_sendFile->setObjectName(QStringLiteral("btn_sendFile"));
        btn_sendFile->setGeometry(QRect(460, 110, 71, 31));
        btn_shaker = new QPushButton(PrivateChatWindow);
        btn_shaker->setObjectName(QStringLiteral("btn_shaker"));
        btn_shaker->setGeometry(QRect(460, 180, 71, 31));
        btn_sendMsg = new QPushButton(PrivateChatWindow);
        btn_sendMsg->setObjectName(QStringLiteral("btn_sendMsg"));
        btn_sendMsg->setGeometry(QRect(440, 410, 91, 31));
        label_historyMsg = new QLabel(PrivateChatWindow);
        label_historyMsg->setObjectName(QStringLiteral("label_historyMsg"));
        label_historyMsg->setGeometry(QRect(60, 20, 54, 12));
        tb_historyMsg = new QTextBrowser(PrivateChatWindow);
        tb_historyMsg->setObjectName(QStringLiteral("tb_historyMsg"));
        tb_historyMsg->setGeometry(QRect(20, 50, 371, 341));
        le_inputMsg = new QLineEdit(PrivateChatWindow);
        le_inputMsg->setObjectName(QStringLiteral("le_inputMsg"));
        le_inputMsg->setGeometry(QRect(20, 410, 391, 31));

        retranslateUi(PrivateChatWindow);

        QMetaObject::connectSlotsByName(PrivateChatWindow);
    } // setupUi

    void retranslateUi(QDialog *PrivateChatWindow)
    {
        PrivateChatWindow->setWindowTitle(QApplication::translate("PrivateChatWindow", "PrivateChatRoom", Q_NULLPTR));
        btn_sendFile->setText(QApplication::translate("PrivateChatWindow", "\344\274\240\350\276\223\346\226\207\344\273\266", Q_NULLPTR));
        btn_shaker->setText(QApplication::translate("PrivateChatWindow", "\346\212\226\345\212\250", Q_NULLPTR));
        btn_sendMsg->setText(QApplication::translate("PrivateChatWindow", "\345\217\221\351\200\201\346\266\210\346\201\257", Q_NULLPTR));
        label_historyMsg->setText(QApplication::translate("PrivateChatWindow", "\345\216\206\345\217\262\346\266\210\346\201\257", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PrivateChatWindow: public Ui_PrivateChatWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVATECHATWINDOW_H

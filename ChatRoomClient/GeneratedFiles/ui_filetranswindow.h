/********************************************************************************
** Form generated from reading UI file 'filetranswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILETRANSWINDOW_H
#define UI_FILETRANSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_FileTransWindow
{
public:
    QLabel *label_filePath;
    QPushButton *btn_choseFile;
    QPushButton *btn_sendFile;
    QProgressBar *Bar_progressFile;
    QLabel *label_fileState;

    void setupUi(QDialog *FileTransWindow)
    {
        if (FileTransWindow->objectName().isEmpty())
            FileTransWindow->setObjectName(QStringLiteral("FileTransWindow"));
        FileTransWindow->resize(675, 276);
        label_filePath = new QLabel(FileTransWindow);
        label_filePath->setObjectName(QStringLiteral("label_filePath"));
        label_filePath->setGeometry(QRect(50, 40, 431, 21));
        btn_choseFile = new QPushButton(FileTransWindow);
        btn_choseFile->setObjectName(QStringLiteral("btn_choseFile"));
        btn_choseFile->setGeometry(QRect(530, 30, 81, 31));
        btn_sendFile = new QPushButton(FileTransWindow);
        btn_sendFile->setObjectName(QStringLiteral("btn_sendFile"));
        btn_sendFile->setGeometry(QRect(500, 200, 81, 31));
        Bar_progressFile = new QProgressBar(FileTransWindow);
        Bar_progressFile->setObjectName(QStringLiteral("Bar_progressFile"));
        Bar_progressFile->setGeometry(QRect(40, 110, 621, 41));
        Bar_progressFile->setValue(24);
        label_fileState = new QLabel(FileTransWindow);
        label_fileState->setObjectName(QStringLiteral("label_fileState"));
        label_fileState->setGeometry(QRect(200, 170, 241, 31));

        retranslateUi(FileTransWindow);

        QMetaObject::connectSlotsByName(FileTransWindow);
    } // setupUi

    void retranslateUi(QDialog *FileTransWindow)
    {
        FileTransWindow->setWindowTitle(QApplication::translate("FileTransWindow", "FileTransWindow", Q_NULLPTR));
        label_filePath->setText(QApplication::translate("FileTransWindow", "\346\226\207\344\273\266\347\233\256\345\275\225", Q_NULLPTR));
        btn_choseFile->setText(QApplication::translate("FileTransWindow", "\347\241\256\345\256\232", Q_NULLPTR));
        btn_sendFile->setText(QApplication::translate("FileTransWindow", "\345\217\221\351\200\201", Q_NULLPTR));
        label_fileState->setText(QApplication::translate("FileTransWindow", "\346\255\243\345\234\250\345\217\221\351\200\201\346\226\207\344\273\266", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FileTransWindow: public Ui_FileTransWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILETRANSWINDOW_H

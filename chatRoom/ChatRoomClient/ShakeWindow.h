
#ifndef SHAKEWINDOW_H
#define SHAKEWINDOW_H

#include <QtGui>
#include <QDialog>
#include <QObject>
#include <QTimer>

//抖动的次数（移动窗口多少次）
enum { MaxLimitTimes = 20 };
//抖动的幅度（每次抖动改变的窗口位置大小）
enum { MaxLimitSpace = 8 };
//抖动定时器发生的时间间隔
enum { ShakeSpeed = 30 };

class ShakeWindow : public QObject
{
	Q_OBJECT

public:
	ShakeWindow(QDialog *dia, QObject *parent = 0);
	~ShakeWindow();
	void startShake();

private:
	QDialog *dia;
	QTimer* m_timer;
	int m_nPosition;
	QPoint m_curPos;

	private slots:
	void slot_timerOut();
};

#endif // SHAKEWINDOW_H


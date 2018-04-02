#include "ShakeWindow.h"

ShakeWindow::ShakeWindow(QDialog *dia, QObject *parent) :QObject(parent)
{
	this->dia = dia;
	m_timer = new QTimer(this);
	QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_timerOut()));
	m_nPosition = MaxLimitTimes;
}

ShakeWindow::~ShakeWindow()
{
	
}

void ShakeWindow::startShake()
{
	//如果正在抖动则返回
	if (m_nPosition < MaxLimitTimes)
		return;
	m_nPosition = 1;
	m_curPos = this->dia->pos();
	m_timer->start(ShakeSpeed);
}

void ShakeWindow::slot_timerOut()
{
	//还没有抖动完毕
	if (m_nPosition < MaxLimitTimes)
	{
		++m_nPosition;
		switch (m_nPosition % 4)
		{
		case 1:
		{
			QPoint tmpPos(m_curPos.x(), m_curPos.y() - MaxLimitSpace);
			this->dia->move(tmpPos);
		}
		break;
		case 2:
		{
			QPoint tmpPos(m_curPos.x() - MaxLimitSpace, m_curPos.y() - MaxLimitSpace);
			this->dia->move(tmpPos);
		}
		break;
		case 3:
		{
			QPoint tmpPos(m_curPos.x() - MaxLimitSpace, m_curPos.y());
			this->dia->move(tmpPos);
		}
		break;
		case 0:
		default:
			this->dia->move(m_curPos);
			break;
		}
	}
	//抖动完毕，关闭定时器
	else
	{
		m_timer->stop();
	}
}

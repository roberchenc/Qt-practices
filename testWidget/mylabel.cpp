#include "mylabel.h"

int MyLabel::ROI_X=0;
int MyLabel::ROI_Y=0;
int MyLabel::ROI_Width=0;
int MyLabel::ROI_Height=0;

MyLabel::MyLabel(QWidget *parent) :
    QLabel(parent)
{

}


void MyLabel::mousePressEvent(QMouseEvent *e)
{
    this->setCursor(Qt::CrossCursor);
    if(e->button() == Qt::LeftButton)
    {
        p2=e->pos();
        p1=e->pos();
    }
    //update();

}

void MyLabel::mouseMoveEvent(QMouseEvent *e)
{
    if(e->button() & Qt::LeftButton)
    {
        p2=e->pos();
    }
}

void MyLabel::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() & Qt::LeftButton)
    {
        p2=e->pos();
        emit clicked();
    }
    update();

}


void MyLabel::paintEvent(QPaintEvent *e)
{


    QLabel::paintEvent(e);
    QPainter painter(this);
    QPen pen; //画笔
    pen.setColor(QColor(255, 0, 0));
    QBrush brush(QColor(0, 255, 0, 125)); //画刷
    painter.setPen(pen); //添加画笔
    painter.setBrush(brush); //添加画刷
    //painter.setPen(QColor(Qt::red));
    ROI_X=p1.x();
    ROI_Y=p1.y();
    ROI_Width=p2.x()-p1.x();
    ROI_Height=p2.y()-p1.y();
    emit position(ROI_X,ROI_Y,ROI_Width,ROI_Height);
    painter.drawRect(ROI_X,ROI_Y,ROI_Width,ROI_Height);

}




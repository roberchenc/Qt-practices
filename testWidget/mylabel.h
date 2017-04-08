#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    //MyLabel();
    explicit MyLabel(QWidget *parent = 0);

signals:
    void clicked();

public slots:

public:
    QPoint p1;
    QPoint p2;

    static int ROI_X;
    static int ROI_Y;
    static int ROI_Width;
    static int ROI_Height;

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void paintEvent(QPaintEvent *);


};

#endif // MYLABEL_H

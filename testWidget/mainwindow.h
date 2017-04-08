#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPainter>
#include <QLabel>
#include <QMouseEvent>
#include <QTimer>
#include <QMainWindow>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void btn_release();


private slots:
   void openCamara();      // 打开摄像头
   void readFarme();       // 读取当前帧信息
   void closeCamara();     // 关闭摄像头。
   void takingPictures();  // 拍照
   void huakuang();

protected:
//    void mousePressEvent(QMouseEvent *e);
//    void mouseMoveEvent(QMouseEvent *e);
//    void mouseReleaseEvent(QMouseEvent *e);
//    void mouseDoubleClickEvent(QMouseEvent *e);

    void paintEvent(QPaintEvent *);

protected:
    //bool eventFilter(QObject* o, QEvent* e);

private:
   QTimer    *timer;
   QImage    *imag;
   CvCapture *cam;// 视频获取结构， 用来作为视频获取函数的一个参数
   IplImage  *frame;//申请IplImage类型指针，就是申请内存空间来存放每一帧图像

   QPixmap pixmap;

   QLabel *statusLabel;
   QLabel *mousePosLabel;

   int startX;
   int startY;
   int width;
   int hight;

   QImage image_capture;
   QImage *image_kuang;
   float kuangW;
   float kuangH;
   bool _needDraw;



private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"
#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QFile>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include "enterexitanimation.h"
typedef struct {
    QColor painterColor;   // 画笔颜色
    qreal  scale;          // 缩放系数
    qreal  translateX;     // 坐标转换 X
    qreal  translateY;     // 坐标转换 Y

    QPoint coordinateHourHand[3];   // 时针形状坐标系
    QPoint coordinateMinuteHand[3]; // 分针形状坐标系
    QPoint coordinateSecondHand[3]; // 秒针形状坐标系

}painterPara;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    QTimer      *timer;
    QLabel      *timeDisplay;
    QLabel      *dateDisplay;
    painterPara  painPara;
    QPoint mousePoint;
    QRectF textRectF(double radius, int pointSize, double angle);
    Enterexitanimation  *enterexitanimation;                                //进出场动画
};

#endif // MAINWINDOW_H

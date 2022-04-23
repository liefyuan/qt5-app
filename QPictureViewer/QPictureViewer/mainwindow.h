#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QTreeWidget>
#include <QListWidget>
#include <QScrollBar>
#include <QHeaderView>
#include <QDir>
#include <QPainter>
#include <QCoreApplication>
#include <QTime>
#include <QMouseEvent>
#include <QDebug>
#include "enterexitanimation.h"


namespace MyAppNS {

enum DynamicUpdateModes {
    ListToPreview,      // 列表模式到预览模式
    ListToFullScreen,   // 列表模式到全屏模式
    PreviewToList,
    PreviewToFullScreen,
    FullScreenToList,
    FullScreenToPreview,
    PreviousOnFullScreen,   // 上一个
    NextOnFullScreen,       // 下一个
    PreviousOnPreview,
    NextOnPreview,
    Scale,               // 缩放
    RotatePositive90,    // 旋转正90
    RotateNegative90     // 旋转负90
};

enum StaticUpdateModes {
    ListMode,        // 列表模式
    PreviewMode,     // 预览模式
    FullScreenMode   // 全屏模式
};

struct imageInfo {
    QString filePath;    // 图像路径
    QString fileName;    // 图像名字
};

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

public slots:
    void slotClickedPbListMode(void);
    void slotClickedPbPreviewMode(void);
    void slotClickedPbFSMode(void);
    void slotCurrentItemChangedIDList(QListWidgetItem *current,
                                      QListWidgetItem *previous);
    void slotItemDoubleClickedIDList(QListWidgetItem *item);
    void slotTimeoutDUTimer(void);
    void slotTimeoutPlayTimer(void);
    void slotTimeoutHMBTimer(void);
    void slotClickedPbPlay(void);
    void slotClickedPbPrevious(void);
    void slotClickedPbNext(void);
    void slotClickedPbEnlarge(void);
    void slotClickedPbReduce(void);
    void slotClickedPbRN90(void);
    void slotClickedPbRP90(void);

private:
    /* 按钮 */
    QWidget *bottomMenuBar;           //  底部菜单栏
    QPushButton *pbPlay;              //  播放图片
    QPushButton *pbEnlarge;           //  放大
    QPushButton *pbReduce;            //  缩小
    QPushButton *pbPrevious;          //  上一张图片
    QPushButton *pbNext;              //  下一张图片
    QPushButton *pbRotatePositive90;  //  旋转正90度
    QPushButton *pbRotateNegative90;  //  旋转负90度
    QPushButton *pbListMode;          //  切换到列表模式
    QPushButton *pbPreviewMode;       //  切换到预览模式
    QPushButton *pbFullScreenMode;    //  切换到全屏模式

    QTimer *dynamicUpdateTimer;       //  动态更新定时器
    QTimer *playTimer;                //  用于实现自动播放的定时器
    QTimer *hideMenuBarTimer;         //  用于实现隐藏菜单栏的定时器
    QLabel *topTitle;                 //  顶部标题
    QListWidget *imageDisplayList;
    QVector<MyAppNS::imageInfo> imageArr;  // 存储图像的数组
    QPoint *lastCoordinate;

    /* 普通私有变量 */
    int currentDUMode;      //  当前的动态更新模式
    int currentSUMode;      //  当前的静态更新模式
    int lastDUMode;         //  上一个动态更新模式
    int dUTimerCount;       //  动态更新定时器计数++
    int imageNum;           //  当前路径下图片的数量
    int leftImageIndex;
    int midImageIndex;
    int rightImageIndex;
    bool dynamicRefreshComplete;
    float currentScaleRatio;        //  当前的缩放系数
    float currentRotationAngle;     //  当前的旋转角度
    QPoint mousePoint;              //  下拉坐标检测

    Enterexitanimation  *enterexitanimation;

    /* 私有方法 */
    void buttonInit(void);
    void imageDisplayListInit(void);
    void setListModeButtonGeometry(void);
    void setPreviewModeButtonGeometry(void);
    void setFullScreenModeButtonGeometry(void);
    void displayButtonInListMode(void);
    void displayButtonInPreviewMode(void);
    void displayButtonInFullScreenMode(void);
    void mySleep(unsigned int msec);
};

#endif // MAINWINDOW_H

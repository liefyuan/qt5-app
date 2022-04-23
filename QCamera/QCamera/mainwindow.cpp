#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置窗口大小，为实际桌面大小
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(0,0,QApplication::desktop()->geometry().width(),QApplication::desktop()->geometry().height());
    //this->setGeometry(0,0,800,480);
    rollingBox = new RollingBox(this);//滚动选择分辨率
    this->setStyleSheet("QMainWindow{border-image:url(./Resource/image/camera)}");
    this->installEventFilter(this);//安装事件过滤器
    cameraProess = new QProcess(this);//实例化进程对象

    rollingBox->setGeometry((float)200/800*this->geometry().width(), (float)350/480*this->geometry().height(),
                            (float)400/800*this->geometry().width(), (float)100/480*this->geometry().height());
    enterexitanimation = new Enterexitanimation(this);
    enterexitanimation->setGeometry(this->geometry().width()-48,0,48,48); //设置位置
    enterexitanimation->setAnimationObject(this);//设置动画对象
}

void MainWindow::startOrExitCamera()
{
    static bool flag = true;
    if(flag){
        cameraProess->kill();
        switch (rollingBox->m_currentValue) {
        case 0:
            cameraProess->start("gst-launch-1.0 -v imxv4l2src device=/dev/video1 ! \"video/x-raw, format=(string)YUY2, width=(int)176, height=(int)144, framerate=(fraction)30/1\" ! imxv4l2sink ");
            break;
        case 1:
            cameraProess->start("gst-launch-1.0 -v imxv4l2src device=/dev/video1 ! \"video/x-raw, format=(string)YUY2, width=(int)320, height=(int)240, framerate=(fraction)30/1\" ! imxv4l2sink ");
            break;
        case 2:
            cameraProess->start("gst-launch-1.0 -v imxv4l2src device=/dev/video1 ! \"video/x-raw, format=(string)YUY2, width=(int)640, height=(int)480, framerate=(fraction)30/1\" ! imxv4l2sink ");
            break;
        case 3:
            cameraProess->start("gst-launch-1.0 -v imxv4l2src device=/dev/video1 ! \"video/x-raw, format=(string)YUY2, width=(int)720, height=(int)480, framerate=(fraction)30/1\" ! imxv4l2sink ");
            break;
        case 4:
            cameraProess->start("gst-launch-1.0 -v imxv4l2src device=/dev/video1 ! \"video/x-raw, format=(string)YUY2, width=(int)720, height=(int)576, framerate=(fraction)30/1\" ! imxv4l2sink ");
            break;
        case 5:
            cameraProess->start("gst-launch-1.0 -v imxv4l2src device=/dev/video1 ! \"video/x-raw, format=(string)YUY2, width=(int)1024, height=(int)768, framerate=(fraction)30/1\" ! imxv4l2sink ");
            break;
        case 6:
            cameraProess->start("gst-launch-1.0 -v imxv4l2src device=/dev/video1 ! \"video/x-raw, format=(string)YUY2, width=(int)1280, height=(int)720, framerate=(fraction)30/1\" ! imxv4l2sink ");
            break;
        case 7://1080p只能采集15帧/s，不能达到30帧/s
            cameraProess->start("gst-launch-1.0 -v imxv4l2src device=/dev/video1 ! \"video/x-raw, format=(string)YUY2, width=(int)1920, height=(int)1080, framerate=(fraction)15/1\" ! imxv4l2sink ");
            break;
        default:
            break;
        }
    }
    else{
        cameraProess->kill();
        cameraProess->start("killall gst-launch-1.0");
    }
    connect(cameraProess,SIGNAL(finished(int)),this,SLOT(slot_cameraProessFinished(int)));
    flag = !flag;
}

void MainWindow::slot_cameraProessFinished(int)
{
    //无
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this){                            //定义事件过滤器，指定在设置列表上检测
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            if(cursor().pos().y()<(float)50/480*this->geometry().height()
                    || cursor().pos().y()>(float)335/480*this->geometry().height())
                return false;
            startOrExitCamera();
            break;
        default:
            break;
        }
    }

    return QWidget::eventFilter(watched,event);//将事件传递给父类
}

MainWindow::~MainWindow()
{
    cameraProess->kill();
    delete ui;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QByteArray>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mainWindowInit();
}

void MainWindow::mainWindowInit()
{
    flag1 = true;
    flag2 = true;

    this->setGeometry(0,0,QApplication::desktop()->screenGeometry().width(),
                      QApplication::desktop()->screenGeometry().height());
    //this->setGeometry(0,0,800,480);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("QMainWindow {border-image:url(./Resource/background/bg)}");


    /* QButtonGroup */
    bgLed1 = new QButtonGroup(this);
    bgLed2 = new QButtonGroup(this);

    connect(bgLed1, SIGNAL(buttonClicked(int)), this, SLOT(execCmd1(int)));
    connect(bgLed2, SIGNAL(buttonClicked(int)), this, SLOT(execCmd2(int)));

    /* label */
    labelLed1 = new QLabel(this);
    labelLed2 = new QLabel(this);
    labelTitle = new QLabel(this);

    labelLed1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labelLed2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labelTitle->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    labelLed1->setText("LED1");
    labelLed2->setText("BEEP");
    labelTitle->setText("板载LED与蜂鸣器控制系统");

    /* led小灯泡图片显示 */
    led1Widget = new QWidget(this);
    led2Widget = new QWidget(this);


    led1Widget->setGeometry((float)120/800*this->geometry().width(), (float)80/480*this->geometry().height(),
                            (float)72/800*this->geometry().width(), (float)72/800*this->geometry().width());
    led2Widget->setGeometry((float)450/800*this->geometry().width(), (float)80/480*this->geometry().height(),
                            (float)72/800*this->geometry().width(), (float)72/800*this->geometry().width());
    labelLed1->setGeometry((float)120/800*this->geometry().width(), (float)200/480*this->geometry().height(),
                           (float)90/800*this->geometry().width(), (float)50/480*this->geometry().height());
    labelLed2->setGeometry((float)450/800*this->geometry().width(), (float)200/480*this->geometry().height(),
                           (float)90/800*this->geometry().width(), (float)50/480*this->geometry().height());

    QFont font;

    font.setPixelSize((float)30/800*this->geometry().width());
    labelTitle->setStyleSheet("QLabel {color: white;}");
    labelTitle->setFont(font);
    labelTitle->setGeometry((float)10/800*this->geometry().width(), 0, (float)400/800*this->geometry().width(),
                            (float)50/480*this->geometry().height());

    font.setPixelSize((float)28/800*this->geometry().width());
    labelLed1->setStyleSheet("QLabel {color: white;}");
    labelLed2->setStyleSheet("QLabel {color: white;}");

    labelLed1->setFont(font);
    labelLed2->setFont(font);

    QString str = QString::number((float)36/800*this->geometry().width());
    int k = 0;
    for (int i = 0; i < 6; i++) {
        QRadioButton *btn = new QRadioButton(this);
        btn->setStyleSheet("QRadioButton::indicator {width: " + str + "px; height: " + str + "px;}"
                                                                                             "QRadioButton::indicator::unchecked {border-image: url(:icon/Resource/icon/btn_unchecked.png);}"
                                                                                             "QRadioButton::indicator::checked {border-image: url(:icon/Resource/icon/btn_checked.png);}"
                                                                                             "QRadioButton {color: white;outline: 0px;}");
        btn->setFont(font);
        if(i<3){
            btn->setGeometry((float)250/800*this->geometry().width(), (float)(130+i*70)/480*this->geometry().height(),
                             (float)100/800*this->geometry().width(), (float)50/480*this->geometry().height());
            bgLed1->addButton(btn, i);
        }else{
            btn->setGeometry((float)580/800*this->geometry().width(), (float)(130+k*70)/480*this->geometry().height(),
                             (float)100/800*this->geometry().width(), (float)50/480*this->geometry().height());
            bgLed2->addButton(btn, k);
            k++;
        }
    }

    bgLed1->button(0)->setText("关闭");
    bgLed1->button(1)->setText("点亮");
    bgLed1->button(2)->setText("闪烁");

    bgLed2->button(0)->setText("关闭");
    bgLed2->button(1)->setText("长叫");
    bgLed2->button(2)->setText("滴嗒");

    timer = new QTimer(this);
    timer->start(200);

    connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeOut()));

    enterexitanimation = new Enterexitanimation(this);
    enterexitanimation->setGeometry(this->geometry().width()-48,0,48,48); //设置位置
    enterexitanimation->setAnimationObject(this);//设置动画对象

}

void MainWindow::execCmd1(int id)
{
    QString path = LED1_PATH;
    /* 由于system函数只能用char类型，所以进行了转换 */
    /* 关 */
    QString cmd_off = "echo 0 >"+ path +"/brightness";
    QByteArray cmdby_off = cmd_off.toLatin1();
    char* charCmd_off = cmdby_off.data();
    /* 开 */
    QString cmd_on = "echo 1 >"+ path +"/brightness";
    QByteArray cmdby_on = cmd_on.toLatin1();
    char* charCmd_on = cmdby_on.data();
    /* 无状态 */
    QString cmd_none = "echo none>"+ path +"/trigger";
    QByteArray cmdby_none = cmd_none.toLatin1();
    char* charCmd_none = cmdby_none.data();
    /* 心跳 */
    QString cmd_heartbeat = "echo heartbeat>"+ path +"/trigger";
    QByteArray cmdby_heartbeat = cmd_heartbeat.toLatin1();
    char* charCmd_heartbeat = cmdby_heartbeat.data();
    switch (id) {
    case 0:
        system(charCmd_off);
        break;

    case 1:
        system(charCmd_none);
        system(charCmd_on);
        break;

    case 2:
        system(charCmd_heartbeat);
        break;
    }
}

void MainWindow::execCmd2(int id)
{
    QString path = LED2_PATH;
    /* 由于system函数只能用char类型，所以进行了转换 */
    /* 关 */
    QString cmd_off = "echo 0 >"+ path +"/brightness";
    QByteArray cmdby_off = cmd_off.toLatin1();
    char* charCmd_off = cmdby_off.data();
    /* 开 */
    QString cmd_on = "echo 1 >"+ path +"/brightness";
    QByteArray cmdby_on = cmd_on.toLatin1();
    char* charCmd_on = cmdby_on.data();
    /* 无状态 */
    QString cmd_none = "echo none>"+ path +"/trigger";
    QByteArray cmdby_none = cmd_none.toLatin1();
    char* charCmd_none = cmdby_none.data();
    /* 心跳 */
    QString cmd_heartbeat = "echo heartbeat>"+ path +"/trigger";
    QByteArray cmdby_heartbeat = cmd_heartbeat.toLatin1();
    char* charCmd_heartbeat = cmdby_heartbeat.data();
    switch (id) {
    case 0:
        system(charCmd_off);
        break;

    case 1:
        system(charCmd_none);
        system(charCmd_on);
        break;

    case 2:
        system(charCmd_heartbeat);
        break;
    }
}

void MainWindow::timerTimeOut()
{
    int id1 = bgLed1->checkedId();
    switch (id1){
    case 0:
        led1Widget->setStyleSheet("QWidget {border-image: url(:icon/Resource/icon/led_off)}");
        break;
    case 1:
        led1Widget->setStyleSheet("QWidget {border-image: url(:icon/Resource/icon/led_on)}");
        break;
    case 2:
        if(flag1)
            led1Widget->setStyleSheet("QWidget {border-image: url(:icon/Resource/icon/led_off)}");
        else
            led1Widget->setStyleSheet("QWidget {border-image: url(:icon/Resource/icon/led_on)}");

        flag1 = !flag1;
        break;
    }

    int id2 = bgLed2->checkedId();
    switch (id2){
    case 0:
        led2Widget->setStyleSheet("QWidget {border-image: url(:icon/Resource/icon/beep_off)}");
        break;
    case 1:
        led2Widget->setStyleSheet("QWidget {border-image: url(:icon/Resource/icon/beep_on)}");
        break;
    case 2:
        if(flag2)
            led2Widget->setStyleSheet("QWidget {border-image: url(:icon/Resource/icon/beep_off)}");
        else
            led2Widget->setStyleSheet("QWidget {border-image: url(:icon/Resource/icon/beep_on)}");
        flag2 = !flag2;
        break;
    }
}

MainWindow::~MainWindow()
{

}



#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include <QDebug>
#include <config.h>
#include <QProcess>
#include <QPixmap>
#include <QGraphicsView>
#include <QFontDatabase>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWindowInit();
    mediaPlayerInit();
    scanSongs();
    currentTimeInit();
    animationInit();
    networkAccessManagerInit();
    weatherDisInit();
}

void MainWindow::mainWindowInit()
{
    this->setGeometry(0,0,QApplication::desktop()->screenGeometry().width(),QApplication::desktop()->screenGeometry().height());
    //this->setGeometry(0,0,800,480);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("QToolTip{border-radius:4;border:1px solid rgb(118, 118, 118); color:white; font-size:15px;}");
    flagAppExec = true;
    scrollingFlag = false;

    //主Wiget，用于设置背景
    mainWidget = new QWidget(this);
    this->setCentralWidget(mainWidget);
    if(this->width()<800)
        mainWidget->setStyleSheet("border-image:url(Resource/background/bg_43)");
    else
        mainWidget->setStyleSheet("border-image:url(Resource/background/bg)");
    mainWidget->installEventFilter(this);

    //桌面音乐板块初始化，控制这里可以把它拖动到任何位置
    musicPlate = new QWidget(mainWidget);
    musicPlate->setGeometry((float)353/800*this->geometry().width(),-(float)5/480*this->geometry().height(),
                            (float)400/800*this->geometry().width(),(float)265/480*this->geometry().height());
    musicPlate->setStyleSheet("border-image:transparent;background-color:transparent;");

    //桌面天气板块初始化，控制这里可以把它拖动到任何位置
    weatherPlate = new QWidget(mainWidget);
    weatherPlate->setGeometry(0,(float)100/480*this->geometry().height(),
                              (float)345/800*this->geometry().width(),(float)195/480*this->geometry().height());
    weatherPlate->setStyleSheet("border-image:transparent;background-color:transparent;");


    appWidgetParent = new QWidget(this);
    appWidgetParent->setGeometry((float)0/800*this->geometry().width(),(float)280/480*this->geometry().height(),
                                 this->geometry().width(),(float)200/800*this->geometry().width());
    appWidgetParent->setStyleSheet("border-image:transparent");

    appWidget = new QWidget(appWidgetParent);
    appWidget->setGeometry(0,0,
                           4*this->geometry().width(),(float)200/800*this->geometry().width());
    appWidget->setStyleSheet("border-image:transparent");
    appWidget->installEventFilter(this);


    loopCount = 0;
    QStringList strList;

    QFont font ( "PangMenZhengDao", 20, 40);
    font.setBold(true);


    font.setPixelSize((float)18/800*this->geometry().width());
    strList<<"音乐"<<"媒体播放"<<"上网"<<"天气"<<"时钟"<<"文件"<<"相册"<<"计算器"<<"阅读"
          <<"记事本"<<"相机"<<"LED"<<"图片浏览器"<<"日历"<<"录音"<<"温度"<<"脚本测试";


    /* 循环动画面板Widget位置与大小确定 */
    for(int i = 0; i < 17; i++){
        loopWidgetArr[i] = new QWidget(appWidget);
        switch (i) {
        case 0://音乐
            loopWidgetArr[i]->setGeometry((float)25/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 1://媒体播放
            loopWidgetArr[i]->setGeometry((float)180/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 2://上网
            loopWidgetArr[i]->setGeometry((float)340/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 3://天气
            loopWidgetArr[i]->setGeometry((float)500/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 4://时钟
            loopWidgetArr[i]->setGeometry((float)655/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 5://文件
            loopWidgetArr[i]->setGeometry((float)825/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 6://相册
            loopWidgetArr[i]->setGeometry((float)980/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 7://计算器
            loopWidgetArr[i]->setGeometry((float)1140/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 8://阅读
            loopWidgetArr[i]->setGeometry((float)1300/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 9://记事本
            loopWidgetArr[i]->setGeometry((float)1455/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 10://相机
            loopWidgetArr[i]->setGeometry((float)1625/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 11://LED
            loopWidgetArr[i]->setGeometry((float)1780/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 12://图片浏览器
            loopWidgetArr[i]->setGeometry((float)1940/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 13://日历
            loopWidgetArr[i]->setGeometry((float)2100/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 14://录音
            loopWidgetArr[i]->setGeometry((float)2255/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 15://温度
            loopWidgetArr[i]->setGeometry((float)2425/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        case 16://脚本测试
            loopWidgetArr[i]->setGeometry((float)2580/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                          (float)120/800*this->geometry().width(),(float)170/480*this->geometry().height());
            break;
        default:
            break;
        }
        iconPushButton[i] = new QPushButton(loopWidgetArr[i]);
        iconPushButton[i]->setGeometry((float)15/800*this->geometry().width(),(float)0/480*this->geometry().height(),
                                       (float)90/800*this->geometry().width(),(float)90/800*this->geometry().width());
        //                loopWidgetArr[i]->setStyleSheet("QWidget{background-color:rgba(80,80,80,0%);"
        //                                                "border-radius: 5px;border-image:transparent}");
        iconPushButton[i]->setStyleSheet("border-image:url(Resource/icon/"+QString::number(i)+".png);background-color:transparent;outline:none;border-radius:8px;");

        loopWidgetArr[i]->setStyleSheet("border-image:transparent;background-color:transparent;outline:none");
        iconPushButton[i]->installEventFilter(this); // 安装事件过滤器
        appNameLabel[i] = new QLabel(loopWidgetArr[i]);
        appNameLabel[i]->setGeometry(0,(float)105/480*this->geometry().height(),loopWidgetArr[i]->geometry().width(),(float)25/480*this->geometry().height());
        appNameLabel[i]->setText(strList[i]);
        appNameLabel[i]->setAlignment(Qt::AlignCenter);
        appNameLabel[i]->setFont(font);
        appNameLabel[i]->setStyleSheet("color:rgba(255,255,255,80%);border-image:transparent;background-color:transparent;outline:none");
    }
    for(int i = 0; i < 4; i++){
        pageWidget[i] = new QWidget(mainWidget);
        if(i==0)
            pageWidget[i]->setStyleSheet("border-image:url(Resource/icon/page2)");
        else
            pageWidget[i]->setStyleSheet("border-image:url(Resource/icon/page1)");
    }
    pageWidget[0]->setGeometry((float)330/800*this->geometry().width(),(float)415/480*this->geometry().height(),
                               (float)15/800*this->geometry().width(),(float)15/800*this->geometry().width());
    pageWidget[1]->setGeometry((float)370/800*this->geometry().width(),(float)415/480*this->geometry().height(),
                               (float)15/800*this->geometry().width(),(float)15/800*this->geometry().width());
    pageWidget[2]->setGeometry((float)410/800*this->geometry().width(),(float)415/480*this->geometry().height(),
                               (float)15/800*this->geometry().width(),(float)15/800*this->geometry().width());
    pageWidget[3]->setGeometry((float)450/800*this->geometry().width(),(float)415/480*this->geometry().height(),
                               (float)15/800*this->geometry().width(),(float)15/800*this->geometry().width());

    appMainInterface = new QLabel(this);
    font.setPixelSize((float)15/800*this->geometry().width());
    appMainInterface->setGeometry((float)0/800*this->geometry().width(),(float)5/480*this->geometry().height(),
                                  (float)150/800*this->geometry().width(),(float)25/480*this->geometry().height());
    appMainInterface->setText("www.openedv.com");
    appMainInterface->setFont(font);
    appMainInterface->setStyleSheet("color:white");
    appMainInterface->setAlignment(Qt::AlignCenter);
    speakersPushbutton = new QPushButton(mainWidget);
    speakersPushbutton->setGeometry((float)750/800*this->geometry().width(),0,(float)50/800*this->geometry().width(),(float)50/800*this->geometry().width());
    speakersPushbutton->setStyleSheet("border-image:url(Resource/icon/settings);outline:none");
    connect(speakersPushbutton,SIGNAL(clicked()),this,SLOT(slotVolumeSpeakers()));

    musicSlider = new QSlider(Qt::Horizontal,musicPlate);
    musicSlider->setGeometry((float)200/800*this->geometry().width(),(float)185/480*this->geometry().height(),
                             (float)185/800*this->geometry().width(),(float)20/480*this->geometry().height());
    musicSlider->setStyleSheet(MUSICSLIDER_STYLE);
    songsPlaySliderPressFlag = false;                                   //是否被按压标志位

    appPathStrList<<"../QMusicPlayer/QMusicPlayer"
                 <<"../QMediaPlayer/QMediaPlayer"
                <<"../QWebBrowser/QWebBrowser"
               <<"../QOnlineWeather/QOnlineWeather"
              <<"../QClock/QClock"
             <<"../QFileView/QFileView"
            <<"../QPictureViewer/QPictureViewer"
           <<"../QCalculator/QCalculator"
          <<"../QReader/QReader"
         <<"../QNotepad/QNotepad"
        <<"../QCamera/QCamera"
       <<"../QLed/QLed"
      <<"../QImageViewer/QImageViewer"
     <<"../QCalendar/QCalendar"
    <<"../QRecorder/QRecorder"
    <<"../QTemperature/QTemperature"
    <<"../QAutotest/QAutotest";
    appExecTimer = new QTimer(this);
    connect(appExecTimer,SIGNAL(timeout()),this,SLOT(slotappExecTimertimeout()));

    /* 消息提示，判断当前目录下是否有这个app */
    messageLabel = new QLabel(this);
    messageLabel->setGeometry((float)350/800*this->geometry().width(),(float)190/480*this->geometry().height(),
                              (float)100/800*this->geometry().width(),(float)100/800*this->geometry().width());
    messageLabel->setStyleSheet("QLabel{border-radius: 5px;background-color:rgba(200,200,200,80%);color:white}");
    messageLabel->setText("该功能暂未上线\n努力优化中...");
    messageLabel->setAlignment(Qt::AlignCenter);
    font.setPixelSize((float)13/800*this->geometry().width());
    messageLabel->setFont(font);
    messageLabel->hide();


    /************************musicPlate(音乐板块)************************************/
    /* 滚动字幕相关 */
    songsName = new TextTicker(musicPlate);
    songsName->setGeometry((float)210/800*this->geometry().width(),(float)120/480*this->geometry().height(),
                           (float)175/800*this->geometry().width(),(float)40/480*this->geometry().height());
    font.setPixelSize((float)18/800*this->geometry().width());
    songsName->setFont(font);
    songsName->setStyleSheet("color:white/*#ffcce6*/");
    //songsName->hide();

    /* 播放进度Label */
    labelDuration = new QLabel(musicPlate);
    font.setPixelSize((float)15/800*this->geometry().width());
    labelDuration->setFont(font);
    labelDuration->setText("00:00/00:00");
    labelDuration->setGeometry((float)275/800*this->geometry().width(),(float)170/480*this->geometry().height(),
                               (float)120/800*this->geometry().width(),(float)20/480*this->geometry().height());
    labelDuration->setStyleSheet("color:white");
    labelDuration->setAlignment(Qt::AlignCenter);

    musicImage = new QLabel(musicPlate);
    musicImage->setGeometry((float)53/800*this->geometry().width(),(float)100/480*this->geometry().height(),
                            (float)150/800*this->geometry().width(),(float)150/800*this->geometry().width());
    //musicImage->setStyleSheet("QLabel{border-image:url(./Resource./images/song.png);}");
    /*是否需要旋转音乐图片！，这里默认是关闭的*/
    musicImage->setVisible(false);
    //musicImage->installEventFilter(this); //禁用事件过滤器，这里是用于旋转音乐图片的，但是考虑到性能问题，不旋转了
    musicTimer = new QTimer(this);
    //connect(musicTimer, SIGNAL(timeout()), this, SLOT( update()));

    playPushButton = new QPushButton(musicPlate);
    playPushButton->setGeometry((float)275/800*this->geometry().width(), (float)195/480*this->geometry().height(),
                                (float)45/800*this->geometry().width(), (float)45/480*this->geometry().height());
    playPushButton->setStyleSheet("QPushButton{border-image:url(Resource/icon/play);outline:none}"
                                  "QPushButton:pressed{border-image:url(Resource/icon/play_p);}");
    connect(playPushButton,SIGNAL(clicked()),this,SLOT(slotPlayPushButtonClicked()));

    previousPushButton = new QPushButton(musicPlate);
    previousPushButton->setGeometry((float)200/800*this->geometry().width(), (float)195/480*this->geometry().height(),
                                    (float)45/800*this->geometry().width(), (float)45/800*this->geometry().width());
    previousPushButton->setStyleSheet("QPushButton{border-image:url(Resource/icon/previousbtn);outline:none}"
                                      "QPushButton:pressed{border-image:url(Resource/icon/previousbtn_p);}");
    connect(previousPushButton,SIGNAL(clicked()),this,SLOT(slotPreviousPushButtonClicked()));

    nextPushButton = new QPushButton(musicPlate);
    nextPushButton->setGeometry((float)350/800*this->geometry().width(), (float)195/480*this->geometry().height(),
                                (float)45/800*this->geometry().width(), (float)45/800*this->geometry().width());
    nextPushButton->setStyleSheet("QPushButton {border-image:url(Resource/icon/nextbtn);outline:none}"
                                  "QPushButton:pressed{border-image:url(Resource/icon/nextbtn_p);}");
    connect(nextPushButton,SIGNAL(clicked()),this,SLOT(slotnextPushButtonClicked()));
    /*********************************************************************************/

    rightWidget = new QWidget(this);
    rightWidget->setGeometry((float)200/800*this->geometry().width(),(float)220/480*this->geometry().height(),
                             (float)400/800*this->geometry().width(),(float)110/480*this->geometry().height());
    rightWidget->setStyleSheet("border-image:transparent;background-color:rgba(40,40,40,50%);border-radius: 20px;");

    speakersSlider = new QSlider(Qt::Horizontal,rightWidget);
    rightWidget->hide();
    speakersSlider->setGeometry((float)25/800*this->geometry().width(),(float)30/480*this->geometry().height(),
                                (float)350/800*this->geometry().width(),(float)50/480*this->geometry().height());
    speakersSlider->setStyleSheet(SPEAKERS_STYLE);
#ifdef __arm__
    speakersSlider->setRange(80,127);
    //因为linux是通过开启进程来控制音量，已经设置阻塞，所以当松手滑块时才开始设置音量
    connect(speakersSlider,SIGNAL(sliderReleased()),this,SLOT(slotSetSpeakerVolume()));
#else
    speakersSlider->setRange(0,100);
    connect(speakersSlider,SIGNAL(valueChanged(int)),this,SLOT(slotSetSpeakerVolume()));
#endif

    vloumeLabel = new QLabel(rightWidget);
    vloumeLabel->setGeometry((float)20/800*this->geometry().width(),(float)10/480*this->geometry().height(),
                             (float)100/800*this->geometry().width(),(float)20/480*this->geometry().height());
    vloumeLabel->setText("扬声器音量");
    vloumeLabel->setAlignment(Qt::AlignCenter);
    vloumeLabel->setStyleSheet("border-image:transparent;background-color:transparent;color:white");
    vloumeLabel->setFont(font);


    // QString pathName = "./Resource./images/song.png";
    QString pathName  = "C:/Users/Administrator/Desktop/QDesktop/QDesktop/Resource/images/song.png";
    sourceImage.load(pathName);
}

void MainWindow::animationInit()
{

    /* 绑定动画对象 */
    propertyAnimation = new QPropertyAnimation(appWidget, "pos");
    propertyAnimation->setDuration(200);//动画长度是0.4s
}

void MainWindow::currentTimeInit()
{
    currentTime     = new QDateTime();
    updateTimer     = new QTimer(this);
    currentTimeLael = new QLabel(mainWidget);
    currentTimeLael2 = new QLabel(mainWidget);
    currentTimeLael3 = new QLabel(mainWidget);
    currentTimeLael4 = new QLabel(weatherPlate);
    QString currentDate =currentTime->currentDateTime().toString("hh:mm");//hh:mm:ss

    //    QFont font;
    QFont font ( "PangMenZhengDao", 20, 40);
    font.setBold(true);
    font.setPixelSize((float)20/800*this->geometry().width());
    currentTimeLael->setStyleSheet("QLabel { color:white;background: transparent;border-image:transparent}");
    currentTimeLael->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    currentTimeLael->setGeometry((float)680/800*this->geometry().width(),(float)6/480*this->geometry().height(),
                                 (float)70/800*this->geometry().width(),(float)20/480*this->geometry().height());
    currentTimeLael->setText(currentDate);
    currentTimeLael->setFont(font);
    currentTimeLael->setAlignment(Qt::AlignCenter);

    currentTimeLael2->setStyleSheet("QLabel { color:white;background: transparent;border-image:transparent}");
    currentTimeLael2->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    currentTimeLael2->setGeometry((float)30/800*this->geometry().width(),(float)60/480*this->geometry().height(),
                                  (float)160/800*this->geometry().width(),(float)50/480*this->geometry().height());
    currentDate =currentTime->currentDateTime().toString("hh:mm");
    currentTimeLael2->setText(currentDate);
    font.setPixelSize((float)50/800*this->geometry().width());
    currentTimeLael2->setFont(font);
    currentTimeLael2->setAlignment(Qt::AlignCenter);

    currentTimeLael3->setStyleSheet("QLabel { color:white;background: transparent;border-image:transparent}");
    currentTimeLael3->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    currentTimeLael3->setGeometry((float)160/800*this->geometry().width(),(float)80/480*this->geometry().height(),
                                  (float)200/800*this->geometry().width(),(float)20/480*this->geometry().height());
    currentDate =currentTime->currentDateTime().toString("yyyy-MM-dd A");
    currentTimeLael3->setText(currentDate);
    font.setPixelSize((float)17/800*this->geometry().width());
    currentTimeLael3->setFont(font);
    currentTimeLael3->setAlignment(Qt::AlignCenter);

    currentTimeLael4->setGeometry((float)100/800*this->geometry().width(),(float)95/480*this->geometry().height(),
                                  (float)200/800*this->geometry().width(),(float)20/480*this->geometry().height());
    currentTimeLael4->setStyleSheet("QLabel { color:rgba(200,200,200,100%);background: transparent;border-image:transparent}");
    currentDate =currentTime->currentDateTime().toString("上次更新时间:hh:mm");
    currentTimeLael4->setText(currentDate);
    font.setPixelSize((float)15/800*this->geometry().width());
    currentTimeLael4->setFont(font);
    currentTimeLael4->setAlignment(Qt::AlignCenter);

    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateTimerTimeOut()));
    updateTimer->start(1000);

}

void MainWindow::updateTimerTimeOut()
{
    QString currentDate =currentTime->currentDateTime().toString("hh:mm");//hh:mm:ss
    currentTimeLael->setText(currentDate);
    currentDate =currentTime->currentDateTime().toString("hh:mm");
    currentTimeLael2->setText(currentDate);
    currentDate =currentTime->currentDateTime().toString("yyyy-MM-dd A");
    currentTimeLael3->setText(currentDate);
}

void MainWindow::forward()
{
    //qDebug()<<"forward"<<loopCount<<endl;
    flagAppExec = false;//app程序禁入标志位
    appExecTimer->start(200);       // 滑动期间200ms内，不进入程序
    static int widthValue = appWidgetParent->geometry().width();
    propertyAnimation->stop();
    propertyAnimation->setStartValue(QPoint(appWidget->pos().x(),0));
    switch (loopCount) {
    case 0:
        propertyAnimation->setEndValue(QPoint(-widthValue*1,0));
        break;
    case 1:
        propertyAnimation->setEndValue(QPoint(-widthValue*2,0));
        break;
    case 2:
        propertyAnimation->setEndValue(QPoint(-widthValue*3,0));
        break;
    default:
        propertyAnimation->setEndValue(QPoint(-widthValue*3,0));
        break;
    }

    propertyAnimation->start();
    loopCount++;

    if(loopCount >= 3)
        loopCount=3;

    for(int i=0;i<4;i++){
        if(i==qAbs(loopCount))
            pageWidget[i]->setStyleSheet("border-image:url(Resource/icon/page2)");
        else
            pageWidget[i]->setStyleSheet("border-image:url(Resource/icon/page1)");
    }
}

void MainWindow::backward()
{
    //qDebug()<<"backward"<<loopCount<<endl;
    flagAppExec = false;//app程序禁入标志位
    appExecTimer->start(200);       // 滑动期间200ms内，不进入程序
    propertyAnimation->stop();
    propertyAnimation->setStartValue(QPoint(appWidget->pos().x(),0));
    static int widthValue = appWidgetParent->geometry().width();
    switch (loopCount) {
    case 1:
        propertyAnimation->setEndValue(QPoint(0,0));
        break;
    case 2:
        propertyAnimation->setEndValue(QPoint(-widthValue,0));
        break;
    case 3:
        propertyAnimation->setEndValue(QPoint(-widthValue*2,0));
        break;
    default:
        propertyAnimation->setEndValue(QPoint(0,0));
        break;
    }
    propertyAnimation->start();
    loopCount--;
    if(loopCount <= 0)
        loopCount=0;
    for(int i=0;i<4;i++){
        if(i==qAbs(loopCount))
            pageWidget[i]->setStyleSheet("border-image:url(Resource/icon/page2)");
        else
            pageWidget[i]->setStyleSheet("border-image:url(Resource/icon/page1)");
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    static QGraphicsOpacityEffect  *opacityEffect = new QGraphicsOpacityEffect(); // 设置图标的不透明度
    static QPoint point;
    static QPoint point2;

    if(/*watched == mainWidget||*/watched == appWidget){
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            // qDebug()<<cursor().pos().x()<<cursor().pos().y()<<endl;
            point.setX(cursor().pos().x());      // 记录按下点的x坐标
            point2.setX(cursor().pos().x());     // 记录按下点的x坐标
            rightWidget->setVisible(false);
            messageLabel->setVisible(false);
            break;
        case QEvent::MouseButtonRelease:
        {
            appExecTimer->start(200);       // 200ms后scrollingFlag为false，这里很重要！
            //qDebug()<<loopCount<<endl;
            int offsetx = cursor().pos().x() - point.x();
            int position = loopCount*appWidgetParent->geometry().width();
            if(qAbs(offsetx) >= 50 && flagAppExec == true){ // 绝对值，求滑动的距离，退出程序1s内禁止滑动
                if(offsetx > 0)
                    backward(); // 向后滚动
                else
                    forward();  // 向前滚动
            }else{
                propertyAnimation->stop();
                propertyAnimation->setStartValue(QPoint(appWidget->pos().x(),0));
                propertyAnimation->setEndValue(QPoint(-position,0));
                propertyAnimation->start();
            }
        }
            break;
        case QEvent::MouseMove:
        {
            int moveOffset = cursor().pos().x() - point2.x();
            appWidget->move(appWidget->pos().x()+moveOffset,0);
            point2.setX(cursor().pos().x());
            appExecTimer->stop();
            scrollingFlag = true;
        }

            break;
        default:
            break;
        }
    }
    for(int i = 0; i < 17; i++){
        if(watched == iconPushButton[i]){
            switch (event->type()) {
            case QEvent::MouseButtonPress:
                loopWidgetArr[i]->setGraphicsEffect(opacityEffect);
                opacityEffect->setOpacity(0.8);         // 按下图标80%透明
                point.setX(cursor().pos().x());         // 记录按下点的x1坐标
                point2.setX(cursor().pos().x());        // 记录按下点的x2坐标
                break;
            case QEvent::Leave:
                opacityEffect->setOpacity(1);           // 鼠标离开后图标为完全不透明
                break;
            case QEvent::MouseButtonRelease:
                appExecTimer->start(200);               // 200ms后scrollingFlag为false，这里很重要！
                opacityEffect->setOpacity(1);           // 松开鼠标后为完全不透明
                if( flagAppExec == true && scrollingFlag == false){//判断屏幕是否在滚动，及退出程序1s内不允许进入
                    QFile file(appPathStrList[i]);
                    if (!file.exists()){                // 判断路径下有没有app程序
                        messageLabel->setVisible(true); // 没有就提示这个app暂时没上线
                        appExecTimer->start(1000);
                    }else{
                        /* 如果正在屏幕还在滚动，那么也不进入程序 */
                        if(propertyAnimation->state() != QPropertyAnimation::Stopped)
                            return true;
                        this->hide();
                        flagAppExec = false;            // 程序进入标志位
                        QProcess appProcess;
                        appProcess.setParent(this);
                        appProcess.start(appPathStrList[i]);
                        appProcess.waitForFinished(-1);
                        appExecTimer->start(1000);      // 程序退出1s后，过滤掉阻塞掉的点击事件，防止再次进入
                        QCursor cursor;
                        cursor.setPos(this->geometry().width(),this->geometry().height());
                        this->show();
                    }
                }
            {
                int offsetx = cursor().pos().x() - point.x();
                //qDebug()<<loopCount<<endl;
                int position = loopCount*appWidgetParent->geometry().width();
                if(qAbs(offsetx) >= 50 && flagAppExec == true){ // 绝对值，求滑动的距离，判断程序退出1s内标志位
                    if(offsetx > 0)
                        backward(); // 向后滚动
                    else
                        forward();  // 向前滚动
                }else{
                    propertyAnimation->stop();
                    propertyAnimation->setStartValue(QPoint(appWidget->pos().x(),0));
                    propertyAnimation->setEndValue(QPoint(-position,0));
                    propertyAnimation->start();
                }

            }

                break;
            case QEvent::MouseMove:
            {
                int moveOffset = cursor().pos().x() - point2.x();
                appWidget->move(appWidget->pos().x()+moveOffset,0);
                point2.setX(cursor().pos().x());
                appExecTimer->stop();
                scrollingFlag = true;//屏幕在滚动！标志位
            }

                break;
            default:
                break;
            }
        }
    }

    if(watched == mainWidget){
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            rightWidget->setVisible(false);
            messageLabel->setVisible(false);
            break;
        default:
            break;
        }
    }

    if (watched == weatherIcon){
        switch (event->type()) {
        case QEvent::Enter:
            weatherIcon->setToolTip("点我刷新天气");
            break;
        case QEvent::MouseButtonPress:
            updateWeather();    // 点击后更新天气
            break;
        default:
            break;
        }
    }

    //音乐旋转控制代码
    if(watched == musicImage && event->type() == QEvent::Paint)
        painter();


    return QWidget::eventFilter(watched,event);//将事件传递给父类
}

void MainWindow::slotVolumeSpeakers()
{
#ifdef __arm__
    if(!flagAppExec)
        return;
    if( getSystemSpeakerVolume().length() > 0){
        QString volume = getSystemSpeakerVolume();
        speakersSlider->setValue(volume.toInt());  //先重新获取一下扬声器音量，防止用户在其他地方有修改过音量
    }else{
        //TODO
    }
#endif
    if(rightWidget->isVisible())
        rightWidget->setVisible(false);
    else
        rightWidget->setVisible(true);
}
//
QString MainWindow::getSystemSpeakerVolume()
{
#ifdef __arm__
    QProcess pro;
    QString cmd = "./Resource/cmd/getSystemSpeakerVolume.sh";
    pro.start(cmd);
    //pro.start("echo $(amixer get Speaker |grep -E \"Left:\" |awk '{print $4}')");
    pro.waitForFinished(-1);
    QString x = pro.readAll();
    return x.left(x.length()-1);
#else
    return "";
#endif
}

void MainWindow::slotSetSpeakerVolume()
{
#ifdef __arm__
    QString volume = getSystemSpeakerVolume();
    if( volume.length()>0){//如果获取到值
        int val = speakersSlider->value();
        if(val <=80)
            val = 0;
        QString cmd = "amixer set Speaker " + QString::number(val) ;
        QProcess pro;
        pro.start(cmd);
        pro.waitForFinished(-1);// 阻塞
    }else{

    }
#else
    mediaPlayer->setVolume(speakersSlider->value());
#endif
}

void MainWindow::slotappExecTimertimeout()
{
    flagAppExec = true;
    scrollingFlag = false;
    appExecTimer->stop();
    messageLabel->hide();/*提示app上暂时没上线1s后关闭*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* 这里原本要设计一个旋转音乐图片的，但是比较耗cpu，6u测试800*480屏，100ms刷新率，占40%cpu，所以这里就不用它了！否则在滑屏的时候不顺畅！*/
void MainWindow::painter()
{
    QPainter painter(musicImage);
    static double angle = 0.0;
    angle += 1;
    if (360 == angle)
        angle = 0;

    static int centerValue = -(float)(sourceImage.width()/4)/480*this->geometry().height();//旋转中心值
    static int imageWidth = (float)(sourceImage.width()/2)/480*this->geometry().height(); //要画的图的边长

    painter.setRenderHint( QPainter::SmoothPixmapTransform,true);
    painter.translate( musicImage->width()/2, musicImage->width()/2 );//设置旋转的中心点
    painter.rotate(angle);

    /* 下行将图片画出旋转 */
    painter.drawImage(QRect(centerValue,centerValue,imageWidth, imageWidth), sourceImage);

}


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWindowInit();
    weatherDisplayInit();
    currentTimeInit();
    networkAccessManagerInit();
    enterexitanimation = new Enterexitanimation(this);
    enterexitanimation->setGeometry(this->geometry().width()-48,0,48,48); //设置位置
    enterexitanimation->setAnimationObject(this);//设置动画对象

}

void MainWindow::mainWindowInit()
{
    this->setGeometry(0,0,QApplication::desktop()->screenGeometry().width(),QApplication::desktop()->screenGeometry().height());
    //this->setGeometry(0,0,800,480);
    this->setWindowFlags(Qt::FramelessWindowHint);

    mainWidget = new QWidget(this);
    mainWidget->installEventFilter(this);
    this->setCentralWidget(mainWidget);
    mainWidget->setStyleSheet("border-image:url(Resource/background/bg)");


    QFont font ( "Microsoft YaHei", 30, 80);
    font.setPointSize((float)20/800*this->geometry().width());

    cityPushButton       = new QPushButton(this);
    cityPushButton->installEventFilter(this);

    cityPushButton->setStyleSheet("QPushButton { color: white;background: transparent;border-image:transparent}");
    cityPushButton->setIcon(QIcon("Resource/icon/position.png"));
    cityPushButton->setIconSize(QSize((float)50/480*this->geometry().height(),(float)50/480*this->geometry().height()));
    cityPushButton->setGeometry((float)200/800*this->geometry().width(),(float)50/480*this->geometry().height(),
                                (float)200/800*this->geometry().width(),(float)50/480*this->geometry().height());
    cityPushButton->setToolTip("点我刷新天气");
    cityPushButton->setText("正在定位...");
    cityPushButton->setToolTipDuration(2000);
    cityPushButton->setFont(font);
    cityPushButton->setFocusPolicy(Qt::NoFocus);

    font.setPointSize((float)30/800*this->geometry().width());
    currentTempLabel = new QLabel(this);
    currentTempLabel->setGeometry((float)250/800*this->geometry().width(),(float)100/480*this->geometry().height(),
                                  (float)120/800*this->geometry().width(),(float)50/480*this->geometry().height());
    currentTempLabel->setText("26℃");
    currentTempLabel->setFont(font);
    currentTempLabel->setStyleSheet("QLabel { color: white;}");

    currentWeatherIcon = new QLabel(this);
    currentWeatherIcon->setGeometry((float)130/800*this->geometry().width(),(float)80/480*this->geometry().height(),
                                    (float)130/800*this->geometry().width(),(float)60/480*this->geometry().height());
    currentWeatherIcon->setStyleSheet("QLabel{border-image:url(./Resource/weatherIcon/晴.png);background-color:transparent}");

    currentWeather = new QLabel(this);
    currentWeather->setGeometry((float)180/800*this->geometry().width(),(float)150/480*this->geometry().height(),
                                (float)50/800*this->geometry().width(),(float)20/480*this->geometry().height());
    currentWeather->setStyleSheet("QLabel { color: white;}");
    currentWeather->setText("晴");
    font.setPointSize((float)13/800*this->geometry().width());
    currentWeather->setFont(font);

    /* 循环动画面板Widget位置与大小确定 */
    for(int i = 0; i < 5; i++){
        loopWidgetArr[i] = new QWidget(mainWidget);
        switch (i) {
        case 3:
            loopWidgetArr[i]->setGeometry((float)30/800*this->geometry().width(),(float)250/480*this->geometry().height(),
                                          (float)130/800*this->geometry().width(),(float)220/480*this->geometry().height());
            break;
        case 4:
            loopWidgetArr[i]->setGeometry((float)185/800*this->geometry().width(),(float)250/480*this->geometry().height(),
                                          (float)130/800*this->geometry().width(),(float)220/480*this->geometry().height());
            break;
        case 0:
            loopWidgetArr[i]->setGeometry((float)338/800*this->geometry().width(),(float)250/480*this->geometry().height(),
                                          (float)130/800*this->geometry().width(),(float)220/480*this->geometry().height());
            break;
        case 1:
            loopWidgetArr[i]->setGeometry((float)490/800*this->geometry().width(),(float)250/480*this->geometry().height(),
                                          (float)130/800*this->geometry().width(),(float)220/480*this->geometry().height());
            break;
        case 2:
            loopWidgetArr[i]->setGeometry((float)642/800*this->geometry().width(),(float)250/480*this->geometry().height(),
                                          (float)130/800*this->geometry().width(),(float)220/480*this->geometry().height());
            break;
        default:
            break;
        }
        loopWidgetArr[i]->setStyleSheet("QWidget{background-color:rgba(80,80,80,40%);"
                                        "border-radius: 5px;border-image:transparent}");
    }

}

void MainWindow::weatherDisplayInit()
{
    /* 天气面板背景初始化，本次设置为透明，
     * 循环面板作为父对象，weatherWidgetArr嵌入loopWidgetArr
     */
    //QFont font;
    QFont font ( "Microsoft YaHei", 30, 80);
    font.setPixelSize((float)13/800*this->geometry().width());
    for(int i=0; i<5; i++){
        weatherWidgetArr[i]     = new QWidget(loopWidgetArr[i]);
        vlayoutArr[i]           = new QVBoxLayout();
        weekLabelArr[i]         = new QLabel(weatherWidgetArr[i]);
        minMaxTempLabelArr[i]   = new QLabel(weatherWidgetArr[i]);
        weatherIconLabelArr[i]  = new QLabel(weatherWidgetArr[i]);
        weatherTypeLabelArr[i]  = new QLabel(weatherWidgetArr[i]);


        /* 绑定动画对象 */
        propertyAnimation[i] = new QPropertyAnimation(loopWidgetArr[i], "geometry");
        propertyAnimation[i]->setDuration(210);//动画长度是0.21s
        /* 设置阴影效果，让循环面板不显生硬 */
        loopGraphicsDropShadowEffect[i] = new QGraphicsDropShadowEffect(this);
        loopGraphicsDropShadowEffect[i]->setOffset(-2,2);
        loopGraphicsDropShadowEffect[i]->setColor(QColor("#888888"));
        loopGraphicsDropShadowEffect[i]->setBlurRadius(30);
        //loopWidgetArr[i]->setGraphicsEffect(loopGraphicsDropShadowEffect[i]);

        /* 天气面板相关设置 */
        weatherWidgetArr[i]->setStyleSheet("QWidget{background-color:transparent}");
        weatherWidgetArr[i]->resize(loopWidgetArr[i]->geometry().width(),loopWidgetArr[i]->geometry().height());

        /* 显示周几相关设置 */
        weekLabelArr[i]->setStyleSheet("QLabel {color: white;background: transparent}");
        weekLabelArr[i]->setAlignment(Qt::AlignCenter);
        weekLabelArr[i]->setText("---\n--");
        weekLabelArr[i]->setFont(font);
        vlayoutArr[i]->addWidget(weekLabelArr[i]);

        /* 最低温度最高温度相关设置 */
        minMaxTempLabelArr[i]->setAlignment(Qt::AlignCenter);
        minMaxTempLabelArr[i]->setText("--℃~--℃");
        minMaxTempLabelArr[i]->setFont(font);
        minMaxTempLabelArr[i]->setStyleSheet("QLabel {color: white;background: transparent}");
        if(i == 0)
            minMaxTempLabelArr[i]->setStyleSheet("QLabel {color: yellow;background: transparent}");
        vlayoutArr[i]->addWidget(minMaxTempLabelArr[i]);

        /* 天气图标相关设置 */
        weatherIconLabelArr[i]->setStyleSheet("QLabel{border-image:url(Resource/weatherIcon/晴.png);background-color:transparent}");
        weatherIconLabelArr[i]->setAlignment(Qt::AlignCenter);
        vlayoutArr[i]->addWidget(weatherIconLabelArr[i]);

        /* 天气类型相关设置 */
        font.setPixelSize((float)13/800*this->geometry().width());
        weatherTypeLabelArr[i]->setStyleSheet("QLabel {color: white;background: transparent}");
        weatherTypeLabelArr[i]->setAlignment(Qt::AlignCenter);
        weatherTypeLabelArr[i]->setText("晴");
        weatherTypeLabelArr[i]->setFont(font);
        vlayoutArr[i]->addWidget(weatherTypeLabelArr[i]);

        /* 将weatherWidgetArr的布局设置成vlayoutArr */
        weatherWidgetArr[i]->setLayout(vlayoutArr[i]);
    }
}

void MainWindow::currentTimeInit()
{
    currentTime     = new QDateTime();
    updateTimer     = new QTimer(this);
    currentTimeLael = new QLabel(mainWidget);
    QString currentDate =currentTime->currentDateTime().toString("yyyy-MM-dd\nhh:mm:ss ddd");

    //QFont font ( "Microsoft YaHei", 30, 80);
    QFont font;
    font.setPixelSize((float)20/800*this->geometry().width());
    currentTimeLael->setStyleSheet("QLabel { color:white;background: transparent;border-image:transparent}");
    currentTimeLael->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    currentTimeLael->setGeometry((float)30/800*this->geometry().width(),(float)190/480*this->geometry().height(),
                                 (float)200/800*this->geometry().width(),(float)50/480*this->geometry().height());
    currentTimeLael->setText(currentDate);
    currentTimeLael->setFont(font);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateTimerTimeOut()));
    updateTimer->start(1000);
}

void MainWindow::updateTimerTimeOut()
{
    QString currentDate =currentTime->currentDateTime().toString("yyyy-MM-dd\nhh:mm:ss ddd");
    currentTimeLael->setText(currentDate);
}

void MainWindow::forwardPushButtonClicked()
{
    if(propertyAnimation[4]->state() == QPropertyAnimation::Stopped){//等待动画完成才可以做下一次的动画
        for(int i = 0; i< 5; i++){
            propertyAnimation[i]->setStartValue(QRect(loopWidgetArr[i]->geometry().x(), loopWidgetArr[i]->geometry().y(),
                                                      loopWidgetArr[i]->geometry().width(), loopWidgetArr[i]->geometry().height())
                                                );
            switch (i) {
            case 0:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[i+1]->geometry().x(), loopWidgetArr[i+1]->geometry().y(),
                        loopWidgetArr[i+1]->geometry().width(), loopWidgetArr[i+1]->geometry().height())
                        );
                break;
            case 1:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[i+1]->geometry().x(), loopWidgetArr[i+1]->geometry().y(),
                        loopWidgetArr[i+1]->geometry().width(), loopWidgetArr[i+1]->geometry().height())
                        );
                break;
            case 2:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[i+1]->geometry().x(), loopWidgetArr[i+1]->geometry().y(),
                        loopWidgetArr[i+1]->geometry().width(), loopWidgetArr[i+1]->geometry().height())
                        );
                break;
            case 3:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[i+1]->geometry().x(), loopWidgetArr[i+1]->geometry().y(),
                        loopWidgetArr[i+1]->geometry().width(), loopWidgetArr[i+1]->geometry().height())
                        );
                break;
            case 4:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[0]->geometry().x(), loopWidgetArr[0]->geometry().y(),
                        loopWidgetArr[0]->geometry().width(), loopWidgetArr[0]->geometry().height())
                        );
                break;
            default:
                break;
            }
            propertyAnimation[i]->start();
            //根据大小更新布局
            loopWidgetArr[i]->setLayout(vlayoutArr[i]);
        }
    }
}

void MainWindow::backwardPushButtonClicked()
{
    if(propertyAnimation[4]->state() == QPropertyAnimation::Stopped){
        for(int i = 0; i< 5; i++){
            propertyAnimation[i]->setStartValue(QRect(loopWidgetArr[i]->geometry().x(), loopWidgetArr[i]->geometry().y(),
                                                      loopWidgetArr[i]->geometry().width(), loopWidgetArr[i]->geometry().height())
                                                );
            switch (i) {
            case 0:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[4]->geometry().x(), loopWidgetArr[4]->geometry().y(),
                        loopWidgetArr[4]->geometry().width(), loopWidgetArr[4]->geometry().height())
                        );
                break;
            case 1:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[i-1]->geometry().x(), loopWidgetArr[i-1]->geometry().y(),
                        loopWidgetArr[i-1]->geometry().width(), loopWidgetArr[i-1]->geometry().height())
                        );
                break;
            case 2:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[i-1]->geometry().x(), loopWidgetArr[i-1]->geometry().y(),
                        loopWidgetArr[i-1]->geometry().width(), loopWidgetArr[i-1]->geometry().height())
                        );
                break;
            case 3:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[i-1]->geometry().x(), loopWidgetArr[i-1]->geometry().y(),
                        loopWidgetArr[i-1]->geometry().width(), loopWidgetArr[i-1]->geometry().height())
                        );
                break;
            case 4:
                propertyAnimation[i]->setEndValue(QRect(loopWidgetArr[i-1]->geometry().x(), loopWidgetArr[i-1]->geometry().y(),
                        loopWidgetArr[i-1]->geometry().width(), loopWidgetArr[i-1]->geometry().height())
                        );
                break;
            default:
                break;
            }
            propertyAnimation[i]->start();
            loopWidgetArr[i]->setLayout(vlayoutArr[i]);
        }
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == cityPushButton && event->type() == QEvent::MouseButtonPress){
        NAManager->clearAccessCache();//清除缓存
        cityPushButton->setText("正在定位...");
        currentTempLabel->setText("--℃");
        for(int i = 0; i < 5; i++){
            weekLabelArr[i]->setText("---\n--");
            minMaxTempLabelArr[i]->setText("--℃~--℃");
            weatherTypeLabelArr[i]->setText("晴");
            weatherIconLabelArr[i]->setStyleSheet("QLabel{border-image:url(Resource/weatherIcon/晴.png);background-color:transparent}");
        }
        //        QNetworkRequest  ipGetNRequest;
        //        ipGetNRequest.setUrl(QUrl(IP_GET_API_URL));
        //        /* get */
        //        QNetworkReply *reply = NAManager->get(ipGetNRequest);
        //        /* 连接信号、槽函数 */
        //        connect(reply, SIGNAL(finished()), this, SLOT(slotFinishedIpGetReply()));
        slotFinishedIpGetReply();
    }
    if(watched == mainWidget ){
        static QPoint point;
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            point.setY(cursor().pos().y());     // 记录按下点的y坐标
            point.setX(cursor().pos().x());     // 记录按下点的x坐标
            break;
        case QEvent::MouseButtonRelease:
        {
            //int offsety = cursor().pos().y() - point.y();
            int offsetx = cursor().pos().x() - point.x();

            if(qAbs(offsetx) > 50){ // 绝对值，求滑动的距离
                if(offsetx < 0)
                    backwardPushButtonClicked(); // 向后滚动
                else
                    forwardPushButtonClicked();  // 向前滚动
            }
        }

            break;
        default:
            break;
        }
    }
    return QWidget::eventFilter(watched,event);//将事件传递给父类
}

MainWindow::~MainWindow()
{
    delete ui;
    NAManager->clearAccessCache();
}

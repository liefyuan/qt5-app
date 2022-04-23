#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QPainter>
#include <QScrollBar>
#include <QFileInfoList>
#include <QDir>
#include <QDebug>
#include <QProcess>
#include <QDateTime>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{          
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->installEventFilter(this);
    this->setStyleSheet("QWidget{border-image:url(./Resource/background/bg.png);}");
    this->setGeometry(0,0,QApplication::desktop()->geometry().width(),QApplication::desktop()->geometry().height());
    //this->setGeometry(0,0,800,480);

    /* 主窗体布局 */
    mainWindowInit();

    /* 按钮初始化 */
    pushButtonInit();

    /* 列表初始化 */
    listWidgetInit();

    /* 滑条初始化 */
    sliderInit();

    /* 媒体相关初始化 */
    mediaplayerInit();

    /* 标签相关初始化 */
    labelInit();

    /* 扫描本地录音文件 */
    scanAudio();

    /* 定时器初始化 */
    timerInit();

    /* 信号槽初始化 */
    processInit();

    /* 信号槽初始化 */
    slotsInit();

    /* 开机默认设置选中0行 */
    recorderListWidget->setCurrentRow(0);

    timer = new QTimer(this);
    timer->start(1500);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerTimeOut()));

    enterexitanimation = new Enterexitanimation(this);
    enterexitanimation->setGeometry(this->geometry().width()-48,0,48,48); //设置位置
    enterexitanimation->setAnimationObject(this);//设置动画对象
}

void MainWindow::mainWindowInit()
{
    /* 主布局 --录音面板 */
    mainWidget = new QWidget(this);
    int size = (float)15/800*this->geometry().width();
    QString strSize = QString::number(size);

    mainWidget->setStyleSheet("QWidget{background-color:#1d2437;border-radius: "+ strSize +"px;border-image:transparent}");
    mainWidget->setGeometry((float)100/800*this->geometry().width(),(float)100/480*this->geometry().height(),
                            (float)600/800*this->geometry().width(),(float)250/480*this->geometry().height());
    //    mainWidget->hide();

    /* 阴影特效 */
    graphicsDropShadowEffect = new QGraphicsDropShadowEffect();
    graphicsDropShadowEffect->setOffset(5,5);
    graphicsDropShadowEffect->setColor(QColor("#444444"));
    graphicsDropShadowEffect->setBlurRadius(30);
    //这里就不设置阴影了,6u性能有限
    //mainWidget->setGraphicsEffect(graphicsDropShadowEffect);

    /* 分割线一 */
    verticalLine1 = new QFrame(mainWidget);
    verticalLine1->setFrameShape(QFrame::VLine);
    verticalLine1->setFrameShadow(QFrame::Sunken);//绘制阴影
    verticalLine1->setGeometry((float)475/600*mainWidget->geometry().width(),(float)60/250*mainWidget->geometry().height(),
                               2,(float)80/250*mainWidget->geometry().height());
    verticalLine1->setStyleSheet("background-color:#111437;");

    /* 分割线二 */
    verticalLine2 = new QFrame(mainWidget);
    verticalLine2->setFrameShape(QFrame::VLine);
    verticalLine2->setFrameShadow(QFrame::Sunken);//绘制阴影
    verticalLine2->setGeometry((float)330/600*mainWidget->geometry().width(),(float)60/250*mainWidget->geometry().height(),
                               2,(float)80/250*mainWidget->geometry().height());
    verticalLine2->setStyleSheet("background-color:#111437;");
}

void MainWindow::pushButtonInit()
{
    QFont font;
    font.setPixelSize((float)13/600*mainWidget->geometry().width());

    /* 录音按钮 */
    recorderButton = new QPushButton(mainWidget);
    recorderButton->setGeometry((float)500/600*mainWidget->geometry().width(),(float)60/250*mainWidget->geometry().height(),
                                (float)80/600*mainWidget->geometry().width(),(float)80/600*mainWidget->geometry().width());
    recorderButton->setStyleSheet(RECORDERBUTTON_STYLE_1);
    recorderButton->setFocusPolicy(Qt::NoFocus);

    /* 麦克风设备QRadioButton */
    micInDevice = new QRadioButton(mainWidget);
    micInDevice->setText("麦克风");
    micInDevice->setChecked(true);
    micInDevice->setGeometry((float)350/600*mainWidget->geometry().width(),(float)75/250*mainWidget->geometry().height(),
                             (float)120/600*mainWidget->geometry().width(),(float)20/250*mainWidget->geometry().height());
    micInDevice->setFont(font);
    int size = (float)25/600*mainWidget->geometry().width();
    QString strSize = QString::number(size);
    micInDevice->setFocusPolicy(Qt::NoFocus);
    micInDevice->setIconSize(QSize((float)28/600*mainWidget->geometry().width(),(float)28/600*mainWidget->geometry().width()));
    micInDevice->setIcon(QIcon("./Resource/icons/micin.png"));
    micInDevice->setStyleSheet("QRadioButton::indicator {width: "+ strSize +"px; height: "+ strSize +"px;}"\
                                                                                                     "QRadioButton::indicator::unchecked {image: url(:icon/Resource/icons/unchecked.png);}"\
                                                                                                     "QRadioButton::indicator::checked {image: url(:icon/Resource/icons/checked.png);}"\
                                                                                                     "QRadioButton{color:white}");

    /* 音频输入设备QRadioButton */
    lineInDevice = new QRadioButton(mainWidget);
    lineInDevice->setGeometry((float)350/600*mainWidget->geometry().width(),(float)100/250*mainWidget->geometry().height(),
                              (float)120/600*mainWidget->geometry().width(),(float)20/250*mainWidget->geometry().height());
    lineInDevice->setFont(font);
    lineInDevice->setText("LineIN");
    lineInDevice->setFocusPolicy(Qt::NoFocus);
    lineInDevice->setIconSize(QSize((float)28/600*mainWidget->geometry().width(),(float)28/600*mainWidget->geometry().width()));
    lineInDevice->setIcon(QIcon("./Resource/icons/linein.png"));
    lineInDevice->setStyleSheet("QRadioButton::indicator {width: "+ strSize +"px; height:"+ strSize +"px;}"\
                                                                                                     "QRadioButton::indicator::unchecked {image: url(:icon/Resource/icons/unchecked.png);}"\
                                                                                                     "QRadioButton::indicator::checked {image: url(:icon/Resource/icons/checked.png);}"\
                                                                                                     "QRadioButton{color:white}");

    /* 播放暂停/上一首/下一首按钮/静音按钮/删除按钮 */
    previousPushButton    = new QPushButton(mainWidget);
    nextPushButton        = new QPushButton(mainWidget);
    playOrPausePushButton = new QPushButton(mainWidget);
    mutePushButton        = new QPushButton(mainWidget);
    deletePushButton      = new QPushButton(mainWidget);

    previousPushButton->setFocusPolicy(Qt::NoFocus);
    nextPushButton->setFocusPolicy(Qt::NoFocus);
    playOrPausePushButton->setFocusPolicy(Qt::NoFocus);
    mutePushButton->setFocusPolicy(Qt::NoFocus);
    deletePushButton->setFocusPolicy(Qt::NoFocus);

    previousPushButton->setGeometry((float)10/600*mainWidget->geometry().width(),(float)200/250*mainWidget->geometry().height(),
                                    (float)40/600*mainWidget->geometry().width(),(float)40/600*mainWidget->geometry().width());
    previousPushButton->setStyleSheet(PREVIOUSPUSHBUTTON_STYLE);
    nextPushButton->setGeometry((float)110/600*mainWidget->geometry().width(),(float)200/250*mainWidget->geometry().height(),
                                (float)40/600*mainWidget->geometry().width(),(float)40/600*mainWidget->geometry().width());
    nextPushButton->setStyleSheet(NEXTPUSHBUTTON_STYLE);
    playOrPausePushButton->setGeometry((float)60/600*mainWidget->geometry().width(),(float)200/250*mainWidget->geometry().height(),
                                       (float)40/600*mainWidget->geometry().width(),(float)40/600*mainWidget->geometry().width());
    playOrPausePushButton->setStyleSheet(PLAYORPAUSEPUSHBUTTON_STYLE_1);
    mutePushButton->setGeometry((float)500/600*mainWidget->geometry().width(),(float)220/250*mainWidget->geometry().height(),
                                (float)20/600*mainWidget->geometry().width(),(float)20/600*mainWidget->geometry().width());
    mutePushButton->setStyleSheet(MUTEPUSHBUTTON_STYLE_1);
    deletePushButton->setGeometry((float)540/600*mainWidget->geometry().width(),(float)220/250*mainWidget->geometry().height(),
                                  (float)20/600*mainWidget->geometry().width(),(float)20/600*mainWidget->geometry().width());
    deletePushButton->setStyleSheet(DELETEPUSHBUTTON_STYLE);

    mutePushButton->installEventFilter(this);
}

void MainWindow::listWidgetInit()
{
    /* 录音列表 */
    recorderListWidget = new QListWidget(mainWidget);
    recorderListWidget->setGeometry(0,(float)10/250*mainWidget->geometry().height(),
                                    (float)300/600*mainWidget->geometry().width(),(float)150/250*mainWidget->geometry().height());
    recorderListWidget->setStyleSheet(RECORDERLISTWIDGET_STYLE);
    recorderListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);        // 关闭水平滚动条
    recorderListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);      // 关闭垂直滚动条
}

void MainWindow::labelInit()
{
    QFont font;
    font.setPixelSize((float)13/600*mainWidget->geometry().width());

    /* 播放长度与音频长度显示 */
    labelDuration = new QLabel(mainWidget);
    labelDuration->setGeometry((float)420/600*mainWidget->geometry().width(),(float)220/250*mainWidget->geometry().height(),
                               (float)80/600*mainWidget->geometry().width(),(float)20/250*mainWidget->geometry().height());
    labelDuration->setText("00:00/00:00");
    labelDuration->setStyleSheet("QLabel{color:#ff5b53;}QLabel{background:transparent}");
    labelDuration->setAlignment(Qt::AlignVCenter);                                //设置对齐方式
    labelDuration->setFont(font);

    /* 音频名称标题显示 */
    labelmediaTitle = new QLabel(mainWidget);
    labelmediaTitle->setGeometry((float)165/600*mainWidget->geometry().width(),(float)200/250*mainWidget->geometry().height(),
                                 (float)200/600*mainWidget->geometry().width(),(float)20/250*mainWidget->geometry().height());
    labelmediaTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labelmediaTitle->setStyleSheet("QLabel{color:#ff5b53;}QLabel{background:transparent}");
    labelmediaTitle->setText("此刻，记录周围美好的声音!");
    labelmediaTitle->setFont(font);

    /* 录音时长显示 */
    labelRecorder = new QLabel(mainWidget);
    labelRecorder->setGeometry((float)523/600*mainWidget->geometry().width(),(float)145/250*mainWidget->geometry().height(),
                               (float)40/600*mainWidget->geometry().width(),(float)20/250*mainWidget->geometry().height());
    labelRecorder->setText("00:00");
    labelRecorder->setStyleSheet("QLabel{color:#ff5b53;}QLabel{background:transparent}");
    labelRecorder->setAlignment(Qt::AlignVCenter);                                //设置对齐方式
    labelRecorder->hide();
    labelRecorder->setFont(font);
}

void MainWindow::sliderInit()
{
    /* 播放进度条初始化 */
    mediaSlider = new QSlider(Qt::Horizontal, mainWidget);          // 设置为水平滑动条
    /* 设置大小位置 */
    mediaSlider->setGeometry((float)160/600*mainWidget->geometry().width(),(float)220/250*mainWidget->geometry().height(),
                             (float)250/600*mainWidget->geometry().width(),(float)32/250*mainWidget->geometry().height());
    mediaSlider->setFocusPolicy(Qt::NoFocus);                       // 去除虚线边框
    mediaSlider->setStyleSheet(MEDIASLIDER_STYLE);                  // 样式表设置
    mediaSlider->setRange(0,0);                                     // 设置范围
    mediaSlider->installEventFilter(this);                          // 安装事件过滤器

    /* 音量条初始化 */
    volumeslider = new QSlider(Qt::Vertical, mainWidget);           // 设置为垂直滑动条
    /* 设置位置与大小 */
    volumeslider->setGeometry((float)500/600*mainWidget->geometry().width(),(float)138/250*mainWidget->geometry().height(),
                              (float)53/600*mainWidget->geometry().width(),(float)75/250*mainWidget->geometry().height());
    volumeslider->setFocusPolicy(Qt::NoFocus);                      // 去除虚线边框
    volumeslider->setStyleSheet(VOLUMESLIDER_STYLE);                // 样式表设置
    volumeslider->setVisible(false);                                // 开机先隐藏
    volumeslider->installEventFilter(this);                         // 安装事件过滤器
    volumeslider->setValue(USER_VOLUME);                            // 设置媒体默认音量
    volumeslider->installEventFilter(this);                         // 安装事件过滤器


}

void MainWindow::timerInit()
{
    timerVolumeSlider = new QTimer(this); // 定时隐藏音量条
    timerRecorder     = new QTimer(this); // 记录录音时长
    recorderDuration  = 0;                // 录音时长初始值为0
}

void MainWindow::processInit()
{
    processCmd = new QProcess(this);      // 用于执行命令，录音指令
}

void MainWindow::switchRecorderDevice()
{
    static bool flag = true;

    /* 判断是否选择麦克风作为音频设备，否则则选择linein作为音频设备 */
    if(micInDevice->isChecked())
        flag = true;
    else
        flag = false;

    if(flag)
        processCmd->start("./Resource/cmd/micinconfig.sh"); //设置为麦克风
    else
        processCmd->start("./Resource/cmd/lineinconfig.sh"); //设置linein


}

void MainWindow::slotsInit()
{
    /* 列表 */
    connect(recorderListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this,SLOT(recorderListWidgetDoubleCliked(QListWidgetItem*)));
    connect(recorderListWidget,SIGNAL(currentRowChanged(int)),
            this,SLOT(recorderListWidgetCurrentRowChanged(int)));

    /* 媒体相关 */
    connect(mediaplayer, SIGNAL(positionChanged(qint64)),this,SLOT(mediaplayerPositionChanged(qint64)));
    connect(mediaplayer, SIGNAL(durationChanged(qint64)),this,SLOT(mediaplayerDurationChanged(qint64)));
    connect(mediaplayer, SIGNAL(stateChanged(QMediaPlayer::State)),
            this,SLOT(mediaplayerStateChanged(QMediaPlayer::State)));
    connect(mediaplayer, SIGNAL(mediaChanged(QMediaContent)),
            this,SLOT(mediaplayerCurrentMediaChanged(QMediaContent)));
    connect(mediaplaylist, SIGNAL(currentIndexChanged(int)),this,SLOT(mediaplaylistCurrentIndexChanged(int)));

    /* 按钮 */
    connect(playOrPausePushButton, SIGNAL(clicked()),this,SLOT(playOrPausePushButtonCliked()));
    connect(nextPushButton, SIGNAL(clicked()),this,SLOT(nextPushButtonCliked()));
    connect(previousPushButton, SIGNAL(clicked()),this,SLOT(previousPushButtonCliked()));
    connect(mutePushButton, SIGNAL(clicked()),this,SLOT(mutePushButtonCliked()));
    connect(deletePushButton, SIGNAL(clicked()),this,SLOT(deleteFile()));
    connect(recorderButton, SIGNAL(clicked()),this,SLOT(recorderButtonCliked()));
    connect(micInDevice,SIGNAL(clicked()),this,SLOT(switchRecorderDevice()));
    connect(lineInDevice,SIGNAL(clicked()),this,SLOT(switchRecorderDevice()));

    /* 滑条 */
    connect(mediaSlider,SIGNAL(sliderPressed()),this,SLOT(mediaSliderPressed()));
    connect(mediaSlider,SIGNAL(sliderReleased()),this,SLOT(mediaSliderReleased()));
    connect(volumeslider,SIGNAL(valueChanged(int)),this,SLOT(volumeSliderValueChanged(int)));

    /* 定时器 */
    connect(timerVolumeSlider,SIGNAL(timeout()),this,SLOT(timerVolumeSliderTimeOut()));
    connect(timerRecorder,SIGNAL(timeout()),this,SLOT(timerRecorderTimeOut()));
}

void MainWindow::timerVolumeSliderTimeOut()
{
    timerVolumeSlider->stop();
    volumeslider->setVisible(false);
}

void MainWindow::timerTimeOut()
{
    timer->stop();
    switchRecorderDevice();//开机默认设置为麦克风

}

MainWindow::~MainWindow()
{
    delete ui;
}

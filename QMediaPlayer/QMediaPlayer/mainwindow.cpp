#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWindowInit();                                               //主窗口大小初始化
    labelInit();                                                    //label标签相关初始化
    mediaPlayerInit();                                              //媒体播放器相关初始化
    scanMedia();                                                    //扫描本地媒体
    pushButtonInit();                                               //按钮相关实例化
    mediaListInit();                                                //媒体列表初始化
    sliderInit();                                                   //进度条相关初始化
    slotsInit();                                                    //信号槽相关初始化
    timerInit();                                                    //定时器相关初始化

    enterexitanimation = new Enterexitanimation(this);
    enterexitanimation->setGeometry(this->geometry().width()-48,0,48,48); //设置位置
    enterexitanimation->setAnimationObject(this);//设置动画对象

    fileChose = new qFileChose(this);
}

void MainWindow::slotsInit()
{
    //列表
    connect(mediaList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(mediaListDoubleCliked(QListWidgetItem*)));
    connect(mediaPlayList,SIGNAL(currentIndexChanged(int)),this,SLOT(mediaPlayListCurrentIndexChanged(int)));

    //媒体
    connect(mediaPlayer,SIGNAL(positionChanged(qint64)),this,SLOT(mediaPlayerPositionChanged(qint64)));
    connect(mediaPlayer,SIGNAL(durationChanged(qint64)),this,SLOT(mediaPlayerDurationChanged(qint64)));
    connect(mediaPlayer,SIGNAL(currentMediaChanged(QMediaContent)),this,SLOT(mediaPlayerCurrentMediaChanged(QMediaContent)));
    connect(mediaPlayer,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(mediaPlayerStateChanged(QMediaPlayer::State)));
    connect(mediaPlayer,SIGNAL(error(QMediaPlayer::Error)),this,SLOT(mediaPlayerError(QMediaPlayer::Error)));
    connect(mediaPlayer,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(mediaPlayerStatusChanged(QMediaPlayer::MediaStatus)));

    //按钮
    connect(playOrPausePushButton,SIGNAL(clicked()),this,SLOT(playOrPausePushButtonCliked()));
    connect(previousPushButton,SIGNAL(clicked()),this,SLOT(previousPushButtonCliked()));
    connect(nextPushButton,SIGNAL(clicked()),this,SLOT(nextPushButtonCliked()));
    connect(openFilePushButton,SIGNAL(clicked()),this,SLOT(openfileButtonCilked()));
    connect(mutePushButton,SIGNAL(clicked()),this,SLOT(mutePushButtonCliked()));

    //滑条
    connect(mediaSlider,SIGNAL(sliderPressed()),this,SLOT(mediaSliderPressed()));
    connect(mediaSlider,SIGNAL(sliderReleased()),this,SLOT(mediaSliderReleased()));
    connect(volumeslider,SIGNAL(valueChanged(int)),this,SLOT(volumeSliderValueChanged(int)));
}

void MainWindow::mainWindowInit()                                   //主窗口大小初始化
{
    this->setWindowFlags(Qt::FramelessWindowHint);                  //设置主窗口为无边框
    //this->setStyleSheet(APP_CONFIG_MAINWINDOW_BACKGROUND_STYLE);    //设置主窗口的背景为透明
    this->installEventFilter(this);                                 //安装事件过滤器

    mainWidget = new QWidget(this);
    mainWidget->setStyleSheet(MAINWIDGET_STYLE_1);                  //这里设置背景为资源里的一张图片


    toolWidgetPanel = new   QWidget(this);
    toolWidgetPanel->setStyleSheet(TOOLWIDGETPANEL_STYLE);          //设置为透明

    /* 在Windows下背景透明是黑色的，正常 */
    toolWidgetPanel->installEventFilter(this);

    this->setGeometry(0,0,QApplication::desktop()->screenGeometry().width(),
                      QApplication::desktop()->screenGeometry().height());
    //this->setGeometry(0,0,480,272);
    //this->setGeometry(0,0,800,480);

    //在设置主窗体大小后再设置子Widget的大小
    toolWidgetPanel->setGeometry(0, (float)405/480*this->geometry().height(),
                                 this->geometry().width(), (float)75/480*this->geometry().height());
    mainWidget->setGeometry(0,0,this->geometry().width(),this->geometry().height());

}

void MainWindow::timerInit()
{
    timerVolumeSlider       = new QTimer(this);
    timerPanelShow          = new QTimer(this);
    connect(timerVolumeSlider,SIGNAL(timeout()),this,SLOT(timerVolumeSliderTimeOut()));
    connect(timerPanelShow,SIGNAL(timeout()),this,SLOT(timerPanelShowTimeOut()));
}

void MainWindow::mediaListInit()                                         //媒体列表初始化
{
    mediaList = new QListWidget(this);
    mediaList->clear();                                                  //清空列表
    mediaList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);       //关闭水平滚动条
    mediaList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);     //关闭垂直滚动条
    mediaList->setFrameShape(QListWidget::NoFrame);                      //去除边框
    mediaList->setGeometry((float)550/800*this->geometry().width(),
                           (float)60/480*this->geometry().height(),
                           (float)250/800*this->geometry().width(),
                           (float)335/480*this->geometry().height()
                           );//0,20,250,385设置媒体列表的位置与大小

    QFont font;
    font.setPointSize((float)12/800*this->geometry().width());
    mediaList->setFont(font);                                            //设置字体
    mediaList->setStyleSheet(SONGSLIST_STYLE);

    QListWidgetItem *item = NULL;
    for (int i = 0; i < mediaObjectInfo.count(); i++) {
        mediaList->addItem(mediaObjectInfo[i].fileName);
        item = mediaList->item(i);
        item->setForeground(Qt::white);
        item->setSizeHint(QSize((float)250/800*this->geometry().width(),
                                (float)30/480*this->geometry().height())
                          );//250，480
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item->setToolTip(mediaObjectInfo[i].fileName);
    }
}

void MainWindow::scanMedia()
{
    QDir dir1(APP_CONFIG_AUDIO_FILE_PATH);
    /* 绝对路径转换，如果不转换则linux底层的gst播放器会找不到文件 */
    QDir dir2(dir1.absolutePath());
    if (dir2.exists()) {                                                    // 如果目录存在
        QStringList filter;                                                 // 定义过滤器
        filter << "*.mp3"<<"*.mkv"<<"*.mp4"<<"*.avi"<<"*.wav";              // 包含所有.**后缀的文件
        QFileInfoList files = dir2.entryInfoList(filter, QDir::Files);      // 获取该目录下的所有文件
        for (int i = 0; i < files.count(); i++) {// 遍历
            MediaObjectInfo info;
            // 使用utf-8编码
            info.fileName = QString::fromUtf8(files.at(i).fileName().toUtf8().data());
            info.filePath = QString::fromUtf8(files.at(i).filePath().toUtf8().data());
            info.fileSuffix = QString::fromUtf8(files.at(i).suffix().toUtf8().data());
            if (mediaPlayList->addMedia(QUrl::fromLocalFile(info.filePath)))//媒体列表添加媒体
                mediaObjectInfo.append(info);                               //添加到容器数组里储存
            else {
                qDebug()<< mediaPlayList->errorString().toUtf8().data() << endl;
                qDebug() << "  Error number:" << mediaPlayList->error() << endl;
                continue;
            }
        }
    }
    else
        //目录如果不存在则创建此目录
        dir1.mkdir(APP_CONFIG_AUDIO_FILE_PATH);
}

//按钮初始化
void MainWindow::pushButtonInit()
{
    previousPushButton    = new QPushButton(toolWidgetPanel);
    nextPushButton        = new QPushButton(toolWidgetPanel);
    playOrPausePushButton = new QPushButton(toolWidgetPanel);
    openFilePushButton    = new QPushButton(toolWidgetPanel);
    mutePushButton        = new QPushButton(toolWidgetPanel);

    //设置位置与大小
    previousPushButton->setGeometry((float)20/800*toolWidgetPanel->geometry().width(), (float)9/75*toolWidgetPanel->geometry().height(),
                                    (float)44/800*toolWidgetPanel->geometry().width(), (float)44/800*toolWidgetPanel->geometry().width());//20,416,44,44
    nextPushButton->setGeometry((float)148/800*toolWidgetPanel->geometry().width(), (float)9/75*toolWidgetPanel->geometry().height(),
                                (float)44/800*toolWidgetPanel->geometry().width(), (float)44/800*toolWidgetPanel->geometry().width());//148,416,44,44
    playOrPausePushButton->setGeometry((float)84/800*toolWidgetPanel->geometry().width(),(float)9/75*toolWidgetPanel->geometry().height(),
                                       (float)44/800*toolWidgetPanel->geometry().width(), (float)44/800*toolWidgetPanel->geometry().width());//84,416,44,44
    openFilePushButton->setGeometry((float)690/800*toolWidgetPanel->geometry().width(), (float)23/75*toolWidgetPanel->geometry().height(),
                                    (float)40/800*toolWidgetPanel->geometry().width(), (float)40/800*toolWidgetPanel->geometry().width());//700,428,20,20
    mutePushButton->setGeometry((float)730/800*toolWidgetPanel->geometry().width(), (float)23/75*toolWidgetPanel->geometry().height(),
                                (float)40/800*toolWidgetPanel->geometry().width(), (float)40/800*toolWidgetPanel->geometry().width());//740,428,20,20

    //属性设置：去除虚线边框
    previousPushButton->setFocusPolicy(Qt::NoFocus);
    nextPushButton->setFocusPolicy(Qt::NoFocus);
    playOrPausePushButton->setFocusPolicy(Qt::NoFocus);
    openFilePushButton->setFocusPolicy(Qt::NoFocus);
    mutePushButton->setFocusPolicy(Qt::NoFocus);

    //设置按钮的样式
    previousPushButton->setStyleSheet(PREVIOUSPUSHBUTTON_STYLE);        //样式表设置
    nextPushButton->setStyleSheet(NEXTPUSHBUTTON_STYLE);
    playOrPausePushButton->setStyleSheet(playOrPausePushButton_STYLE_1);
    openFilePushButton->setStyleSheet(OPENFILEPUSHBUTTON_STYLE);
    mutePushButton->setStyleSheet(MUTEPUSHBUTTON_STYLE_1);

    mutePushButton->installEventFilter(this);
}

void MainWindow::sliderInit()
{
    mediaSliderPressFlag = false;                                   //是否被按压标志位

    //播放进度条
    mediaSlider = new QSlider(Qt::Horizontal, toolWidgetPanel);     //设置为水平滑动条
    mediaSlider->setGeometry((float)200/800*toolWidgetPanel->geometry().width(),
                             (float)36/75*toolWidgetPanel->geometry().height(),
                             (float)350/800*toolWidgetPanel->geometry().width(),
                             (float)35/75*toolWidgetPanel->geometry().height());//212,441,350,200设置位置与大小
    mediaSlider->setFocusPolicy(Qt::NoFocus);                       //去除虚线边框
    mediaSlider->setStyleSheet(MEDIASLIDER_STYLE);                  //样式表设置
    mediaSlider->setRange(0,0);
    mediaSlider->installEventFilter(this);                          //安装事件过滤器

    //音量条
    volumeslider = new QSlider(Qt::Vertical, this);                 //设置为垂直滑动条
    volumeslider->setGeometry((float)725/800*this->geometry().width(),
                              (float)302/480*this->geometry().height(),
                              (float)52/800*this->geometry().width(),
                              (float)121/480*this->geometry().height());//740,302,20,121设置位置与大小
    volumeslider->setFocusPolicy(Qt::NoFocus);                          //去除虚线边框
    volumeslider->setStyleSheet(VOLUMESLIDER_STYLE);                    //样式表设置
    volumeslider->setVisible(false);                                    //开机先隐藏
    volumeslider->installEventFilter(this);                             //安装事件过滤器
    volumeslider->setValue(APP_CONFIG_PLAYER_DEFAULT_VOLUME);           //设置媒体默认音量

}

void MainWindow::labelInit()
{
    QFont font;
    font.setPointSize((float)14/800*this->geometry().width());

    labelDuration = new QLabel(toolWidgetPanel);                       //播放进度标签
    labelDuration->setFont(font);
    labelDuration->setGeometry((float)570/800*toolWidgetPanel->geometry().width(),
                               (float)35/75*toolWidgetPanel->geometry().height(),
                               (float)120/800*toolWidgetPanel->geometry().width(),
                               (float)20/75*toolWidgetPanel->geometry().height());//相对位置(570,440,120,20)
    labelDuration->setText("00:00/00:00");
    labelDuration->setStyleSheet("QLabel{color:white;}QLabel{background:transparent}");
    labelDuration->setAlignment(Qt::AlignVCenter);          //设置对齐方式


    labelmediasTitle = new QLabel(toolWidgetPanel);                     //媒体标题（标签）
    labelmediasTitle->setGeometry((float)212/800*toolWidgetPanel->geometry().width(),
                                  (float)11/75*toolWidgetPanel->geometry().height(),
                                  (float)350/800*toolWidgetPanel->geometry().width(),
                                  (float)25/75*toolWidgetPanel->geometry().height());//212,416,350,25
    labelmediasTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labelmediasTitle->setStyleSheet("QLabel{color:white;}QLabel{background:transparent}");
    labelmediasTitle->setText("当前无播放媒体");
    labelmediasTitle->setFont(font);

    musicLabel = new QLabel(this);                     //媒体标题（标签）
    musicLabel->setGeometry((float)200/800*this->geometry().width(),
                            (float)200/480*this->geometry().height(),
                            (float)200/800*this->geometry().width(),
                            (float)25/480*this->geometry().height());//350,200,350,25
    musicLabel->setText("音乐 请欣赏！");
    musicLabel->setStyleSheet("QLabel{color:white;}QLabel{background:transparent}");
    musicLabel->setFont(font);
    musicLabel->setAlignment(Qt::AlignCenter);          //设置对齐方式

    musicLabel->hide();
}


void MainWindow::openfileButtonCilked()
{
    //下面注释的这些，本来是打开一个文件夹对话框去选择多个文件的，后面因为考虑到是触摸屏设备，每次点击只能打开一个文件添加到列表
    int count = mediaObjectInfo.count();//获取当前储存的媒体信息总数
    //调用系统打开文件窗口，设置窗口标题为“添加媒体文件”，过滤文件名
    //QStringList fileNames = QFileDialog::getOpenFileNames(0,tr("添加媒体文件"),"/opt/media/",
    //                                                     tr("Files(*.mp3 *.mp4 *.wav *.avi *.mkv)"), 0, QFileDialog::DontUseNativeDialog|QFileDialog::ReadOnly);
    QString fileNames = fileChose->choseOpenFileName(this,tr("添加媒体文件"),"/opt/media/",
                                                          tr("Files(*.mp3 *.mp4 *.wav *.avi *.mkv)"), QFileDialog::DontUseNativeDialog|QFileDialog::ReadOnly);
//    for (int i = 0; i < fileNames.count(); i++)
//    {
        //QFile file(fileNames.at(i));
        QFile file(fileNames);
        if (!file.exists())
            return;
        QFileInfo fileInfo(file);
        MediaObjectInfo info;
        info.fileName = QString::fromUtf8(fileInfo.fileName().toUtf8().data()); // 使用utf-8编码
        info.filePath = QString::fromUtf8(fileInfo.filePath().toUtf8().data());
        info.fileSuffix = QString::fromUtf8(fileInfo.suffix().toUtf8().data());
        if (mediaPlayList->addMedia(QUrl::fromLocalFile(info.filePath)))
            mediaObjectInfo.append(info);
//    }

    QListWidgetItem *item = NULL;
//    for(int i = 0; i < fileNames.count(); i++){//添加项
//        mediaList->addItem(mediaObjectInfo[count + i].fileName);
     mediaList->addItem(mediaObjectInfo[count].fileName);
//    }

    for (int i = count; i <mediaList->count(); i++) {//为后面添加的项设项样式
        item = mediaList->item(i);
        item->setForeground(Qt::white);
        item->setSizeHint(QSize((float)250/800*this->geometry().width(),
                                (float)30/480*this->geometry().height())
                          );//250，480
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item->setToolTip(mediaObjectInfo[i].fileName);
    }
}
void MainWindow::timerVolumeSliderTimeOut()
{
    timerVolumeSlider->stop();
    volumeslider->setVisible(false);
}


void MainWindow::timerPanelShowTimeOut()
{
    timerPanelShow->stop();
    if(!mediaList->isVisible()){
        toolWidgetPanel->hide();
    }
}

void MainWindow::enterAnimation()
{
    enterexitanimation->setAnimationObject(this);
}
MainWindow::~MainWindow()
{
    delete ui;
}

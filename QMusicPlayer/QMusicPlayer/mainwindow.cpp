#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include <QGraphicsOpacityEffect>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWindowInit();                                               //主窗口大小初始化
    labelInit();                                                    //label标签相关初始化
    iconInit();                                                     //图标初始化
    mediaPlayerInit();                                              //媒体播放器相关初始化
    playBackModeListInit();                                         //播放模式列表初始化
    settingsListInit();                                             //设置列表初始化
    scanSongs();                                                    //扫描本地歌曲
    pushButtonInit();                                               //按钮相关实例化
    sliderInit();                                                   //进度条相关初始化
    songsListInit();                                                //歌曲列表初始化
    slotsInit();                                                    //信号槽相关初始化
    timerInit();                                                    //定时器相关初始化

    enterexitanimation = new Enterexitanimation(this);
    enterexitanimation->setGeometry(this->geometry().width()-48,0,48,48); //设置位置
    enterexitanimation->setAnimationObject(this);//设置动画对象
}

void MainWindow::slotsInit()
{
    //列表
    connect(songList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(songListCliked(QListWidgetItem*)));
    connect(playBackModeList,SIGNAL(itemEntered(QListWidgetItem*)),this,SLOT(playbackModeListItemEnter(QListWidgetItem*)));
    connect(playBackModeList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(playbackModeListItemCliked(QListWidgetItem*)));
    connect(playBackModeList,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(playbackModeListCurrentItemChange(QListWidgetItem*,QListWidgetItem*)));
    connect(settingsList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(settingsListItemCliked(QListWidgetItem*)));
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
    connect(playbackMode,SIGNAL(clicked()),this,SLOT(playbackModeCliked()));
    connect(mutePushButton,SIGNAL(clicked()),this,SLOT(mutePushButtonCliked()));

    //滑条
    connect(songsPlaySlider,SIGNAL(sliderPressed()),this,SLOT(songsPlaySliderPressed()));
    connect(songsPlaySlider,SIGNAL(sliderReleased()),this,SLOT(songsPlaySliderReleased()));
    connect(volumeslider,SIGNAL(valueChanged(int)),this,SLOT(volumeSliderValueChanged(int)));

}

void MainWindow::mainWindowInit()                                   //主窗口大小初始化
{
    this->setWindowFlags(Qt::FramelessWindowHint);                  //设置主窗口为无边框
    this->setStyleSheet(APP_CONFIG_MAINWINDOW_BACKGROUND_IMAGE);    //设置主窗口的背景
    this->installEventFilter(this);                                 //安装事件过滤器
    this->setGeometry(0,0,QApplication::desktop()->screenGeometry().width(),
                      QApplication::desktop()->screenGeometry().height());
    //this->setGeometry(0,0,480,272);
}

void MainWindow::timerInit()
{
    timerVolumeSlider       = new QTimer(this);
    timerSongList           = new QTimer(this);
    connect(timerVolumeSlider,SIGNAL(timeout()),this,SLOT(timerVolumeSliderTimeOut()));
    connect(timerSongList,SIGNAL(timeout()),this,SLOT(timerSongListTimeOut()));
    timerSongList->start(200);

}

void MainWindow::iconInit()
{
    pbModeArr.clear();
    PbMode mode;

    mode.name = "单曲播放";
    mode.icon1 = "Resource/icon/btn_listsingle_1.png";
    mode.icon2 = "Resource/icon/btn_listsingle_2.png";
    pbModeArr.append(mode);

    mode.name = "单曲循环";
    mode.icon1 = "Resource/icon/btn_listscircle_single_1.png";
    mode.icon2 = "Resource/icon/btn_listscircle_single_2.png";
    pbModeArr.append(mode);

    mode.name = "顺序播放";
    mode.icon1 = "Resource/icon/btn_listjump_1.png";
    mode.icon2 = "Resource/icon/btn_listjump_2.png";
    pbModeArr.append(mode);

    mode.name = "列表循环";
    mode.icon1 = "Resource/icon/btn_listcircle_1.png";
    mode.icon2 = "Resource/icon/btn_listcircle_2.png";
    pbModeArr.append(mode);

    mode.name = "随机播放";
    mode.icon1 = "Resource/icon/btn_listrandom_1.png";
    mode.icon2 = "Resource/icon/btn_listrandom_2.png";
    pbModeArr.append(mode);

    setModeArr.clear();
    setMode set;

    set.name = "关闭写真";
    set.icon1 = "Resource/icon/photo_1.png";
    set.icon2 = "Resource/icon/photo_2.png";
    setModeArr.append(set);

    set.name = "界面设置";
    set.icon1 = "Resource/icon/settings_1.png";
    set.icon2 = "Resource/icon/settings_2.png";
    setModeArr.append(set);
}

void MainWindow::songsListInit()                                        //歌曲列表初始化
{
    songList = new QListWidget(this);
    songList->clear();                                                  //清空列表
    songList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);       //关闭水平滚动条
    songList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);     //关闭垂直滚动条
    songList->setFrameShape(QListWidget::NoFrame);                      //去除边框
    songList->setGeometry(0,
                          (float)45/480*this->geometry().height(),
                          (float)250/800*this->geometry().width(),
                          (float)350/480*this->geometry().height()
                          );//0,20,250,385设置歌曲列表的位置与大小

    QFont font;
    font.setPointSize((float)12/800*this->geometry().width());
    songList->setFont(font);                                            //设置字体
    songList->setStyleSheet(SONGSLIST_STYLE);

    QListWidgetItem *item = NULL;
    for (int i = 0; i < mediaObjectInfo.count(); i++) {
        songList->addItem(mediaObjectInfo[i].fileName);
        item = songList->item(i);
        item->setForeground(Qt::white);
        item->setSizeHint(QSize((float)250/800*this->geometry().width(),
                                (float)30/480*this->geometry().height())
                          );//250，480
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
    songList->setVisible(false);
}

void MainWindow::playBackModeListInit()
{
    QFont font;
    font.setPointSize((float)12/800*this->geometry().width());

    playBackModeList    = new QListWidget(this);            //媒体播放模式列表
    playBackModeList->clear();
    playBackModeList->setVisible(false);                    //开机隐藏播放模式列表
    playBackModeList->installEventFilter(this);
    playBackModeList->setGeometry((float)660/800*this->geometry().width(),
                                  (float)273/480*this->geometry().height(),
                                  (float)100/800*this->geometry().width(),
                                  (float)150/480*this->geometry().height());//660,273,100,150
    playBackModeList->setStyleSheet(PLAYBACKMODELIST_STYLE);
    playBackModeList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);                       //去掉垂直滑块
    playBackModeList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);                     //去掉水平滑块
    playBackModeList->setFrameShape(QListWidget::NoFrame);                                      //隐藏外边框
    playBackModeList->setFont(font);
    playBackModeList->setMouseTracking(true);                                                   //开启鼠标跟踪检测，需要开启这个，itementer事件才有效
    playBackModeList->setIconSize(QSize((float)20/800*this->geometry().width(),
                                        (float)20/480*this->geometry().height())
                                  );

    QListWidgetItem *item = NULL;
    for (int i = 0; i < pbModeArr.count(); i++) {
        playBackModeList->addItem(pbModeArr[i].name);
        item = playBackModeList->item(i);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item->setSizeHint(QSize((float)111/800*this->geometry().width(),
                                (float)30/480*this->geometry().height())
                          );
        item->setForeground(QColor(210, 210, 210));
        item->setIcon(QIcon(pbModeArr[i].icon1));
    }
    playBackModeList->setCurrentRow(APP_CONFIG_PLAYER_DEFAULT_PLAYBACK_MODE);                   //初始化时设置当选active的行

}

void MainWindow::settingsListInit()
{
    QFont font;
    font.setPointSize(12);

    settingsList = new QListWidget(this);
    settingsList->setGeometry(this->geometry().width()-100,this->geometry().height()/2-50,100,60);
    settingsList->setStyleSheet(SETTINGS_STYLE);
    settingsList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);                       //去掉垂直滑块
    settingsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);                     //去掉水平滑块
    settingsList->setFrameShape(QListWidget::NoFrame);                                      //隐藏外边框
    settingsList->setIconSize(QSize(20, 20));
    settingsList->setFont(font);
    settingsList->installEventFilter(this);
    settingsList->setFocusPolicy(Qt::NoFocus);

    QListWidgetItem *item = NULL;
    for (int i = 0; i < setModeArr.count(); i++) {
        settingsList->addItem(setModeArr[i].name);
        item = settingsList->item(i);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item->setSizeHint(QSize(111, 30));
        item->setForeground(QColor(200, 200, 200));
        item->setIcon(QIcon(setModeArr[i].icon1));
    }
}

void MainWindow::scanSongs()
{
    QDir dir1(APP_CONFIG_AUDIO_FILE_PATH);
    //绝对路径转换，如果不转换则linux底层的播放器会找不到文件
    QDir dir2(dir1.absolutePath());
    if (dir2.exists()) {                                                    // 如果目录存在
        QStringList filter;                                                 // 定义过滤器
        filter << "*.mp3";                                                  // 包含所有.mp3后缀的文件
        QFileInfoList files = dir2.entryInfoList(filter, QDir::Files);      // 获取该目录下的所有文件
        for (int i = 0; i < files.count(); i++) {// 遍历
            MediaObjectInfo info;
            // 使用utf-8编码
            info.fileName = QString::fromUtf8(files.at(i).fileName().replace(".mp3", "").toUtf8().data());
            info.filePath = QString::fromUtf8(files.at(i).filePath().toUtf8().data());
            if (mediaPlayList->addMedia(QUrl::fromLocalFile(info.filePath)))//媒体列表添加歌曲
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

    /* 设置播放器开机默认的播放模式 */
    switch (APP_CONFIG_PLAYER_DEFAULT_PLAYBACK_MODE) {
    case 0:
        mediaPlayList->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        break;

    case 1:
        mediaPlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        break;

    case 2:
        mediaPlayList->setPlaybackMode(QMediaPlaylist::Sequential);
        break;

    case 3:
        mediaPlayList->setPlaybackMode(QMediaPlaylist::Loop);
        break;

    case 4:
        mediaPlayList->setPlaybackMode(QMediaPlaylist::Random);
        break;

    default:
        mediaPlayList->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    }
}

void MainWindow::pushButtonInit()
{
    previousPushButton    = new QPushButton(this);
    nextPushButton        = new QPushButton(this);
    playOrPausePushButton = new QPushButton(this);
    playbackMode          = new QPushButton(this);
    mutePushButton        = new QPushButton(this);

    //设置位置与大小
    previousPushButton->setGeometry((float)20/800*this->geometry().width(), (float)416/480*this->geometry().height(),
                                    (float)44/800*this->geometry().width(), (float)44/800*this->geometry().width());//20,416,44,44
    nextPushButton->setGeometry((float)148/800*this->geometry().width(), (float)416/480*this->geometry().height(),
                                (float)44/800*this->geometry().width(), (float)44/800*this->geometry().width());//148,416,44,44
    playOrPausePushButton->setGeometry((float)84/800*this->geometry().width(),(float)416/480*this->geometry().height(),
                                       (float)44/800*this->geometry().width(), (float)44/800*this->geometry().width());//84,416,44,44
    playbackMode->setGeometry((float)690/800*this->geometry().width(), (float)418/480*this->geometry().height(),
                              (float)40/800*this->geometry().width(), (float)40/800*this->geometry().width());//700,428,20,20
    mutePushButton->setGeometry((float)730/800*this->geometry().width(), (float)418/480*this->geometry().height(),
                                (float)40/800*this->geometry().width(), (float)40/800*this->geometry().width());//740,428,20,20

    //属性设置：去除虚线边框
    previousPushButton->setFocusPolicy(Qt::NoFocus);
    nextPushButton->setFocusPolicy(Qt::NoFocus);
    playOrPausePushButton->setFocusPolicy(Qt::NoFocus);
    playbackMode->setFocusPolicy(Qt::NoFocus);
    mutePushButton->setFocusPolicy(Qt::NoFocus);

    //设置按钮的样式
    previousPushButton->setStyleSheet(PREVIOUSPUSHBUTTON_STYLE);        //样式表设置
    nextPushButton->setStyleSheet(NEXTPUSHBUTTON_STYLE);
    playOrPausePushButton->setStyleSheet(playOrPausePushButton_STYLE_1);
    mutePushButton->setStyleSheet(MUTEPUSHBUTTON_STYLE_1);

    mutePushButton->installEventFilter(this);

    //开机播放模式设置，默认是单曲播放，可以在config.h文件里设置默认播放模式
    switch (APP_CONFIG_PLAYER_DEFAULT_PLAYBACK_MODE) {
    case 0:
        playbackMode->setStyleSheet(PLAYBACKMODE_STYLE_0);
        break;

    case 1:
        playbackMode->setStyleSheet(PLAYBACKMODE_STYLE_1);
        break;

    case 2:
        playbackMode->setStyleSheet(PLAYBACKMODE_STYLE_2);
        break;

    case 3:
        playbackMode->setStyleSheet(PLAYBACKMODE_STYLE_3);
        break;

    case 4:
        playbackMode->setStyleSheet(PLAYBACKMODE_STYLE_4);
        break;
        //如果设置错误 默认使用列表循环播放模式
    default:
        playbackMode->setStyleSheet(PLAYBACKMODE_STYLE_3);
        break;
    }
}

void MainWindow::sliderInit()
{
    songsPlaySliderPressFlag = false;                                   //是否被按压标志位

    //播放进度条
    songsPlaySlider = new QSlider(Qt::Horizontal, this);                //设置为水平滑动条
    songsPlaySlider->setGeometry((float)212/800*this->geometry().width(),
                                 (float)441/480*this->geometry().height(),
                                 (float)350/800*this->geometry().width(),
                                 (float)35/480*this->geometry().height());//212,441,350,25设置位置与大小
    songsPlaySlider->setFocusPolicy(Qt::NoFocus);                       //去除虚线边框
    songsPlaySlider->setStyleSheet(SONGSPLAYSLIDER_STYLE);              //样式表设置
    songsPlaySlider->setRange(0,0);
    songsPlaySlider->installEventFilter(this);                          //安装事件过滤器

    //音量条
    volumeslider = new QSlider(Qt::Vertical, this);                     //设置为垂直滑动条
    volumeslider->setGeometry((float)725/800*this->geometry().width(),
                              (float)302/480*this->geometry().height(),
                              (float)50/800*this->geometry().width(),
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

    LabelMainWindowBackground = new QLabel(this);           //用于覆盖桌面写真背景，防止背景过亮

    LabelMainWindowBackground->setStyleSheet("background-color: rgba(0, 0, 0, 40%);");
    LabelMainWindowBackground->resize(this->geometry().width(),this->geometry().height());

    labelDuration = new QLabel(this);                       //播放进度标签
    labelDuration->setFont(font);
    labelDuration->setGeometry((float)570/800*this->geometry().width(),
                               (float)440/480*this->geometry().height(),
                               (float)120/800*this->geometry().width(),
                               (float)20/480*this->geometry().height());//570,440,120,20
    labelDuration->setText("00:00/00:00");
    labelDuration->setStyleSheet("QLabel{color:white;}QLabel{background:transparent}");
    labelDuration->setAlignment(Qt::AlignVCenter);          //设置对齐方式


    labelSongsTitle = new QLabel(this);                     //歌曲标题（标签）
    labelSongsTitle->setGeometry((float)212/800*this->geometry().width(),
                                 (float)416/480*this->geometry().height(),
                                 (float)350/800*this->geometry().width(),
                                 (float)25/480*this->geometry().height());//212,416,350,25
    labelSongsTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labelSongsTitle->setStyleSheet("QLabel{color:white;}QLabel{background:transparent}");
    labelSongsTitle->setText("Q音乐 极致享受!");
    labelSongsTitle->setFont(font);

    //初始化歌词标签文本
    labelArr.clear();
    for (int i = 0; i < 7; i++) {
        QLabel *label = new QLabel(this);
        labelArr.append(label);
    }

    font.setPointSize((float)18/800*this->geometry().width());
    for (int i = 0; i < 7; i++) {
        labelArr[i]->setFont(font);
        labelArr[i]->setStyleSheet("QLabel{color:white;}QLabel{background:transparent}");
        labelArr[i]->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
    labelArr[3]->setStyleSheet("QLabel{color:#faff02;}QLabel{background:transparent}");

    labelArr[3]->setText("Q音乐 极致享受!");                  //设置歌词文本初始显示内容

    for (int i = 0; i < 7; i++){
        labelArr[i]->setGeometry((float)250/800*this->geometry().width(),
                                 (float)(50 + i*40)/480*this->geometry().height(),
                                 (float)550/800*this->geometry().width(),
                                 (float)30/480*this->geometry().height());//250, (50 + i*40), 550, 30
    }
}

void MainWindow::timerVolumeSliderTimeOut()
{
    timerVolumeSlider->stop();
    volumeslider->setVisible(false);
}

void MainWindow::timerSongListTimeOut()
{
    static QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(songList);
    static int count = 0;
    static double op = 0.0;
    songList->setVisible(true);
    songList->setGraphicsEffect(opacityEffect);
    opacityEffect->setOpacity(op);
    count++;
    if(count >= 10){
        op = op +0.1;
        if(op >+ 1.0)
            op = 1.0;
    }
    if(count >= 20)
        timerSongList->stop();

}

MainWindow::~MainWindow()
{
    delete ui;
}

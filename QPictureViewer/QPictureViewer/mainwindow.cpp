#include "mainwindow.h"
#include <QDesktopWidget>
#include <QApplication>
#define  DEFAULT_WORK_DIR   "Resource/images"
#define  ONCE_SCALE_RATIO   0.2

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    /* 主窗口设置 */
    this->setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height());
    //this->setGeometry(0,0,480,272);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("QWidget {background-color: rgb(0, 0, 0);}");
    this->setMouseTracking(true);
    
    /* 顶部标题栏 */
    topTitle = new QLabel(this);
    topTitle->setGeometry(0, 0, this->geometry().width(), 40);
    topTitle->setStyleSheet("QLabel {background-color: rgb(50, 50, 50); font-size: 14px; color: white;}");
    topTitle->setFrameShape(QLabel::NoFrame);
    topTitle->setText("相册");
    topTitle->setAlignment(Qt::AlignCenter);

    /* 缩略图显示列表初始化 */
    imageDisplayListInit();

    enterexitanimation = new Enterexitanimation(this);

    /* 按钮初始化 */
    buttonInit();
    
    /* 定时器初始化 */
    dynamicUpdateTimer = new QTimer(this);
    playTimer = new QTimer(this);
    hideMenuBarTimer = new QTimer(this);
    connect(dynamicUpdateTimer, SIGNAL(timeout()), this, SLOT(slotTimeoutDUTimer()));
    connect(playTimer, SIGNAL(timeout()), this, SLOT(slotTimeoutPlayTimer()));
    connect(hideMenuBarTimer, SIGNAL(timeout()), this, SLOT(slotTimeoutHMBTimer()));
    
    /* 普通私有变量初始化 */
    currentSUMode = MyAppNS::ListMode;
    dUTimerCount = 0;
    lastDUMode = -1;
    currentDUMode = -1;
    dynamicRefreshComplete = true;
    
    leftImageIndex = imageNum - 1 >= 0? imageNum - 1: -1;
    midImageIndex = imageNum - 1 >= 0? 0: -1;
    rightImageIndex = imageNum - 2 >= 0? 1: midImageIndex;
    
    currentScaleRatio = 1.0;      //  表示当前没有缩放
    currentRotationAngle = 0.0;   //  表示当前没有进行角度旋转
    lastCoordinate = new QPoint;

    //    enterexitanimation = new Enterexitanimation(this);
    enterexitanimation->setGeometry(this->geometry().width()-48,0,48,48); //设置位置
    enterexitanimation->setAnimationObject(this);//设置动画对象

}

MainWindow::~MainWindow()
{

}


/*
 * 私有方法
 */
void MainWindow::buttonInit(void) {
    /* 实例化 */
    bottomMenuBar = new QWidget(this);
    pbListMode = new QPushButton(bottomMenuBar);
    pbPreviewMode = new QPushButton(bottomMenuBar);
    pbFullScreenMode = new QPushButton(bottomMenuBar);
    pbPlay = new QPushButton(bottomMenuBar);
    pbEnlarge = new QPushButton(bottomMenuBar);
    pbReduce = new QPushButton(bottomMenuBar);
    pbPrevious = new QPushButton(bottomMenuBar);
    pbNext = new QPushButton(bottomMenuBar);
    pbRotatePositive90 = new QPushButton(bottomMenuBar);
    pbRotateNegative90 = new QPushButton(bottomMenuBar);

    /* 确定控件坐标 */
    bottomMenuBar->setGeometry(0, this->geometry().height()-50, this->geometry().width(), 50);
    setListModeButtonGeometry();
    
    /* 样式表设置 */
    bottomMenuBar->setStyleSheet("QWidget {background-color: rgba(50, 50, 50, 100%); }");
    pbListMode->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/listMode1)}"
                              "QPushButton:hover {border-image: url(:/icon/Resource/listMode2)}"
                              "QPushButton:pressed {border-image: url(:/icon/Resource/listMode1)}"
                              "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                              );
    pbPreviewMode->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/previewMode1)}"
                                 "QPushButton:hover {border-image: url(:/icon/Resource/previewMode2)}"
                                 "QPushButton:pressed {border-image: url(:/icon/Resource/previewMode1)}"
                                 "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                                 );
    pbFullScreenMode->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/fullScreen1)}"
                                    "QPushButton:hover {border-image: url(:/icon/Resource/fullScreen2)}"
                                    "QPushButton:pressed {border-image: url(:/icon/Resource/fullScreen1)}"
                                    "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                                    );
    pbPlay->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/play1)}"
                          "QPushButton:hover {border-image: url(:/icon/Resource/play2)}"
                          "QPushButton:pressed {border-image: url(:/icon/Resource/play1)}"
                          "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                          );
    pbEnlarge->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/enlarge1)}"
                             "QPushButton:hover {border-image: url(:/icon/Resource/enlarge2)}"
                             "QPushButton:pressed {border-image: url(:/icon/Resource/enlarge1)}"
                             "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                             );
    pbReduce->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/reduce1)}"
                            "QPushButton:hover {border-image: url(:/icon/Resource/reduce2)}"
                            "QPushButton:pressed {border-image: url(:/icon/Resource/reduce1)}"
                            "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                            );
    pbPrevious->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/previous1)}"
                              "QPushButton:hover {border-image: url(:/icon/Resource/previous2)}"
                              "QPushButton:pressed {border-image: url(:/icon/Resource/previous1)}"
                              "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                              );
    pbNext->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/next1)}"
                          "QPushButton:hover {border-image: url(:/icon/Resource/next2)}"
                          "QPushButton:pressed {border-image: url(:/icon/Resource/next1)}"
                          "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                          );
    pbRotateNegative90->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/rotateNegative90_1)}"
                                      "QPushButton:hover {border-image: url(:/icon/Resource/rotateNegative90_2)}"
                                      "QPushButton:pressed {border-image: url(:/icon/Resource/rotateNegative90_1)}"
                                      "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                                      );
    pbRotatePositive90->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/rotatePositive90_1)}"
                                      "QPushButton:hover {border-image: url(:/icon/Resource/rotatePositive90_2)}"
                                      "QPushButton:pressed {border-image: url(:/icon/Resource/rotatePositive90_1)}"
                                      "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                                      );
    
    /* 启动进入列表模式，隐藏其他模式下的按钮 */
    displayButtonInListMode();
    
    /* 信号、槽函数连接 */
    connect(pbListMode, SIGNAL(clicked()), this, SLOT(slotClickedPbListMode()));
    connect(pbPreviewMode, SIGNAL(clicked()), this, SLOT(slotClickedPbPreviewMode()));
    connect(pbFullScreenMode, SIGNAL(clicked()), this, SLOT(slotClickedPbFSMode()));
    connect(pbPlay, SIGNAL(clicked()), this, SLOT(slotClickedPbPlay()));
    connect(pbPrevious, SIGNAL(clicked()), this, SLOT(slotClickedPbPrevious()));
    connect(pbNext, SIGNAL(clicked()), this, SLOT(slotClickedPbNext()));
    connect(pbEnlarge, SIGNAL(clicked()), this, SLOT(slotClickedPbEnlarge()));
    connect(pbReduce, SIGNAL(clicked()), this, SLOT(slotClickedPbReduce()));
    connect(pbRotateNegative90, SIGNAL(clicked()), this, SLOT(slotClickedPbRN90()));
    connect(pbRotatePositive90, SIGNAL(clicked()), this, SLOT(slotClickedPbRP90()));
}

void MainWindow::imageDisplayListInit(void) {
    /* 控件实例化、位置确定 */
    imageDisplayList = new QListWidget(this);
    imageDisplayList->setGeometry(0, 40, this->geometry().width(), (float)420/480*this->geometry().height());
    imageDisplayList->clear();
    
    /* 属性设置 */
    imageDisplayList->setViewMode(QListView::IconMode);    //  设置显示模式为图标模式
    imageDisplayList->setIconSize(QSize((float)180/800*this->geometry().width(), (float)85/480*this->geometry().height())); //  设置图标的大小
    imageDisplayList->setSpacing((float)12/800*this->geometry().width());                //  设置两个相邻图标的间距
    imageDisplayList->setMovement(QListWidget::Static);    //  设置图标不能移动
    imageDisplayList->setResizeMode(QListWidget::Adjust);  //  设置自动适应布局调整（默认不适应）
    imageDisplayList->setFrameShape(QListWidget::NoFrame); //  去除边框
    imageDisplayList->setFocusPolicy(Qt::NoFocus);         //  去除选中时候的虚线边框
    
    /* 垂直滑块样式表设置 */
    imageDisplayList->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {width:10px; background:rgba(0,0,0,0%);}"
                                                         "QScrollBar::handle:vertical {width:10px; background:rgba(255,255,255,15%); border-radius:4px;}"
                                                         "QScrollBar::handle:vertical:hover {width:10px; background:rgba(255,255,255,40%); border-radius:4px;}"
                                                         "QScrollBar::sub-line:vertical {width:0px; height:0px;}"
                                                         "QScrollBar::add-line:vertical {width:0px; height:0px;}"
                                                         "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical {background:rgba(0,0,0,0%);}");
    
    /* 显示默认列表中的图像 */
    QDir dir(DEFAULT_WORK_DIR);
    imageArr.clear();
    if (dir.exists()) {
        
        QStringList filter;             // 定义一个过滤器
        filter << "*.png" << "*.jpg" << "*.jpeg";   // 添加需要扫描的文件类型
        QFileInfoList files = dir.entryInfoList(filter, QDir::Files);
        
        for (int i = 0; i < files.count(); i++) {
            MyAppNS::imageInfo image;
            image.fileName = files.at(i).fileName();
            image.filePath = files.at(i).filePath();
            imageArr.append(image);      //  将image路径添加到数组
            
            /* 添加缩略图 */
            QListWidgetItem *item = new QListWidgetItem;
            item->setIcon(QIcon(image.filePath));
            item->setText(image.fileName);
            item->setTextColor(Qt::white);
            item->setSizeHint(QSize((float)180/800*this->geometry().width(),
                                    (float)109/800*this->geometry().width())    //设置每次个item的大小
                              );
            imageDisplayList->addItem(item);
        }
    }
    
    /* 获取数量 */
    imageNum = imageArr.count();
    
    /* 信号、槽连接 */
    connect(imageDisplayList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(slotCurrentItemChangedIDList(QListWidgetItem*,QListWidgetItem*)));
    connect(imageDisplayList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotItemDoubleClickedIDList(QListWidgetItem*)));
}

void MainWindow::setListModeButtonGeometry(void) {
    pbPlay->setGeometry((float)310/800*this->geometry().width(), 0, 50, 50);
    pbPreviewMode->setGeometry((float)370/800*this->geometry().width(), 0, 50, 50);
    pbFullScreenMode->setGeometry((float)430/800*this->geometry().width(), 0, 50, 50);
}

void MainWindow::setPreviewModeButtonGeometry(void) {
    pbPrevious->setGeometry((float)300/800*this->geometry().width(), 0, 50, 50);
    pbPlay->setGeometry((float)140/800*this->geometry().width(), 0, 50, 50);
    pbNext->setGeometry((float)370/800*this->geometry().width(), 0, 50, 50);
    pbListMode->setGeometry((float)440/800*this->geometry().width(), 0, 50, 50);
    pbFullScreenMode->setGeometry((float)510/800*this->geometry().width(), 0, 50, 50);
}

void MainWindow::setFullScreenModeButtonGeometry(void) {
    pbPrevious->setGeometry((float)300/800*this->geometry().width(), 0, 50, 50);
    pbReduce->setGeometry((float)230/800*this->geometry().width(), 0, 50, 50);
    pbEnlarge->setGeometry((float)160/800*this->geometry().width(), 0, 50, 50);
    pbPlay->setGeometry((float)90/800*this->geometry().width(), 0, 50, 50);
    pbNext->setGeometry((float)370/800*this->geometry().width(), 0, 50, 50);
    pbRotateNegative90->setGeometry((float)440/800*this->geometry().width(), 0, 50, 50);
    pbRotatePositive90->setGeometry((float)490/800*this->geometry().width(), 0, 50, 50);
    pbListMode->setGeometry((float)580/800*this->geometry().width(), 0, 50, 50);
    pbPreviewMode->setGeometry((float)650/800*this->geometry().width(), 0, 50, 50);
}

void MainWindow::displayButtonInListMode(void) {
    topTitle->setVisible(true);
    imageDisplayList->setVisible(true);
    bottomMenuBar->setVisible(true);
    
    pbListMode->setVisible(false);
    pbEnlarge->setVisible(false);
    pbReduce->setVisible(false);
    pbPrevious->setVisible(false);
    pbNext->setVisible(false);
    pbRotateNegative90->setVisible(false);
    pbRotatePositive90->setVisible(false);
    
    pbPreviewMode->setVisible(true);
    pbFullScreenMode->setVisible(true);
    enterexitanimation->setVisible(true);
}

void MainWindow::displayButtonInPreviewMode(void) {
    topTitle->setVisible(false);
    imageDisplayList->setVisible(false);
    bottomMenuBar->setVisible(true);
    
    pbPreviewMode->setVisible(false);
    pbEnlarge->setVisible(false);
    pbReduce->setVisible(false);
    pbRotateNegative90->setVisible(false);
    pbRotatePositive90->setVisible(false);
    
    pbListMode->setVisible(true);
    pbFullScreenMode->setVisible(true);
    pbPrevious->setVisible(true);
    pbNext->setVisible(true);
    enterexitanimation->setVisible(false);
}

void MainWindow::displayButtonInFullScreenMode(void) {
    topTitle->setVisible(false);
    imageDisplayList->setVisible(false);
    bottomMenuBar->setVisible(false);
    pbFullScreenMode->setVisible(false);
    
    pbEnlarge->setVisible(true);
    pbReduce->setVisible(true);
    pbPrevious->setVisible(true);
    pbNext->setVisible(true);
    pbRotateNegative90->setVisible(true);
    pbRotatePositive90->setVisible(true);
    pbListMode->setVisible(true);
    pbPreviewMode->setVisible(true);
    enterexitanimation->setVisible(false);
}

void MainWindow::mySleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

/*
 * 槽函数
 */
void MainWindow::slotClickedPbListMode(void) {
    if (!dynamicRefreshComplete)
        return;
    
    /* 配置按钮位置 */
    setListModeButtonGeometry();
    
    /* 显示 */
    displayButtonInListMode();
    bottomMenuBar->setStyleSheet("QWidget {background-color: rgba(50, 50, 50, 100%); }");
    
    /* 关闭隐藏菜单栏的定时器 */
    hideMenuBarTimer->stop(); // 如果是从全屏模式下进入预览模式 这个定时器基本是打开的 必须关闭
    
    /* 如果当前正在播放 则停止 关闭定时器 并设置图标 */
    if (playTimer->isActive()) {
        playTimer->stop();
        pbPlay->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton:hover {border-image: url(:/icon/Resource/play2)}"
                              "QPushButton:pressed {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                              );
    }
    
    /* 设置模式 ****/
    /* 这里没有做 从其他模式切换到列表模式的动态效果 所以将其设置为-1 */
    currentSUMode = MyAppNS::ListMode;
    currentDUMode = -1;
    
    /* 从其他模式切换到列表模式的时候 必须重置index */
    leftImageIndex = imageNum - 1 >= 0? imageNum - 1: -1;
    midImageIndex = imageNum - 1 >= 0? 0: -1;
    rightImageIndex = imageNum - 2 >= 0? 1: midImageIndex;
    
    /* 重置缩放系数、旋转角度 */
    currentRotationAngle = 0;
    currentScaleRatio = 1.0;
    
    /* 标志位置位 */
    dynamicRefreshComplete = false;
    dynamicUpdateTimer->start(50);
}

void MainWindow::slotClickedPbPreviewMode(void) {
    if (!dynamicRefreshComplete || 0 == imageNum)
        return;
    
    /* 设置按钮位置 */
    setPreviewModeButtonGeometry();
    
    /* 隐藏按钮 */
    displayButtonInPreviewMode();
    bottomMenuBar->setStyleSheet("QWidget {background-color: rgba(50, 50, 50, 100%); }");
    
    /* 关闭隐藏菜单栏的定时器 */
    hideMenuBarTimer->stop(); // 如果是从全屏模式下进入预览模式 这个定时器基本是打开的 必须关闭
    
    /* 如果当前正在播放 则停止 关闭定时器 */
    if (playTimer->isActive()) {
        playTimer->stop();
        pbPlay->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton:hover {border-image: url(:/icon/Resource/play2)}"
                              "QPushButton:pressed {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                              );
    }
    
    /* 设置模式 ****/
    switch (currentSUMode) {
    
    case MyAppNS::ListMode:
        currentDUMode = MyAppNS::ListToPreview;
        break;
        
    case MyAppNS::FullScreenMode:
        currentDUMode = MyAppNS::FullScreenToPreview;
        break;
        
    default: break;
    }
    
    currentSUMode = MyAppNS::PreviewMode;
    
    /* 重置缩放系数、旋转角度 */
    currentRotationAngle = 0;
    currentScaleRatio = 1.0;
    
    /* 标志位置位 */
    dynamicRefreshComplete = false;
    dynamicUpdateTimer->start(50);
}

void MainWindow::slotClickedPbFSMode(void) {
    if (!dynamicRefreshComplete || 0 == imageNum)
        return;
    
    /* 显示全屏模式下的控件并重置坐标 */
    setFullScreenModeButtonGeometry();
    displayButtonInFullScreenMode();
    bottomMenuBar->setStyleSheet("QWidget {background-color: rgba(50, 50, 50, 50%); }");
    
    /* 设置模式 */
    switch (currentSUMode) {
    
    case MyAppNS::ListMode:
        currentDUMode = MyAppNS::ListToFullScreen;
        break;
        
    case MyAppNS::PreviewMode:
        currentDUMode = MyAppNS::PreviewToFullScreen;
        break;
        
    default: break;
    }
    
    currentSUMode = MyAppNS::FullScreenMode;
    
    /* 开启定时器 */
    dynamicRefreshComplete = false;
    dynamicUpdateTimer->start(50);
}

void MainWindow::slotCurrentItemChangedIDList(QListWidgetItem *current,
                                              QListWidgetItem *previous)
{
    if (NULL == current) {
        leftImageIndex = imageNum - 1 >= 0? imageNum - 1: -1;
        midImageIndex = imageNum - 1 >= 0? 0: -1;
        rightImageIndex = imageNum - 2 >= 0? 1: midImageIndex;
    }
    else {
        midImageIndex = imageDisplayList->row(current);
        leftImageIndex = midImageIndex - 1 >= 0? midImageIndex - 1: imageNum - 1;
        rightImageIndex = midImageIndex + 1 < imageNum? midImageIndex + 1: 0;
    }
    Q_UNUSED(previous);
}

void MainWindow::slotItemDoubleClickedIDList(QListWidgetItem *item) {
    if (!dynamicRefreshComplete || NULL == item)
        return;
    
    /* 确定index */
    midImageIndex = imageDisplayList->row(item);
    leftImageIndex = midImageIndex - 1 >= 0? midImageIndex - 1: imageNum - 1;
    rightImageIndex = midImageIndex + 1 < imageNum? midImageIndex + 1: 0;
    
    /* 控件隐藏 */
    displayButtonInFullScreenMode();
    
    /* 设置位置 */
    setFullScreenModeButtonGeometry();
    
    /* 设置菜单栏的透明度 */
    bottomMenuBar->setStyleSheet("QWidget {background-color: rgba(50, 50, 50, 50%); }");
    
    /* 设置模式 */
    currentSUMode = MyAppNS::FullScreenMode;
    currentDUMode = MyAppNS::ListToFullScreen;
    
    /* 开启定时器 */
    dynamicRefreshComplete = false;
    dynamicUpdateTimer->start(50);
}

void MainWindow::slotTimeoutDUTimer(void) {
    dUTimerCount++;
    this->update();
}

void MainWindow::slotTimeoutPlayTimer(void) {
    /* 这个定时器一到 意味着需要切换当前播放的图片了 */
    /* 计算index */
    leftImageIndex = midImageIndex;
    midImageIndex = midImageIndex + 1 < imageNum? midImageIndex + 1: 0;
    rightImageIndex = midImageIndex + 1 < imageNum? midImageIndex + 1: 0;
    
    /* 重新开启动态刷新定时器 */
    dynamicRefreshComplete = false;
    dynamicUpdateTimer->start(50);
}

void MainWindow::slotTimeoutHMBTimer(void) {
    /* 停止运行定时器 */
    hideMenuBarTimer->stop();
    
    /* 赶紧给我藏起来！别被我发现了，狗日的！ */
    bottomMenuBar->setVisible(false);
}

void MainWindow::slotClickedPbPlay(void) {
    if (!dynamicRefreshComplete || 0 == imageNum)
        return;
    
    /* 如果正在播放 则停止 */
    if (playTimer->isActive()) {
        playTimer->stop();
        pbPlay->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton:hover {border-image: url(:/icon/Resource/play2)}"
                              "QPushButton:pressed {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                              );
        return;                 // 需要在所有退出播放的事件中重置为false
    }
    
    /* 更换图标: stop */
    pbPlay->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/stop1)}"
                          "QPushButton:hover {border-image: url(:/icon/Resource/stop2)}"
                          "QPushButton:pressed {border-image: url(:/icon/Resource/stop1)}"
                          "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                          );
    
    if (MyAppNS::FullScreenMode != currentSUMode) {
        /* 隐藏控件 */
        displayButtonInFullScreenMode();
        
        /* 设置位置 */
        setFullScreenModeButtonGeometry();
        bottomMenuBar->setStyleSheet("QWidget {background-color: rgba(50, 50, 50, 50%); }");
    }
    
    if (MyAppNS::FullScreenMode == currentSUMode) {
        leftImageIndex = midImageIndex;
        midImageIndex = midImageIndex + 1 < imageNum? midImageIndex + 1: 0;
        rightImageIndex = midImageIndex + 1 < imageNum? midImageIndex + 1: 0;
        
        /* 重置缩放系数、旋转角度 */
        currentRotationAngle = 0;
        currentScaleRatio = 1.0;
    }
    
    /* 标志位初始化 */
    currentSUMode = MyAppNS::FullScreenMode;
    currentDUMode = MyAppNS::NextOnFullScreen;
    
    /* 标志位置位 */
    dynamicRefreshComplete = false;
    
    /* 开启定时器 */
    dynamicUpdateTimer->start(50);     //  开启动态刷新定时器
    playTimer->start(2000);            //  开启图片切换定时器
}

void MainWindow::slotClickedPbPrevious(void) {
    if (!dynamicRefreshComplete)
        return;
    
    /* 如果正在播放则停止播放 */
    if (playTimer->isActive()) {
        playTimer->stop();
        pbPlay->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton:hover {border-image: url(:/icon/Resource/play2)}"
                              "QPushButton:pressed {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                              );
    }
    
    /* 更新index */
    rightImageIndex = midImageIndex;
    midImageIndex = leftImageIndex;
    leftImageIndex = leftImageIndex - 1 >= 0? leftImageIndex - 1: imageNum - 1;
    
    /*更新模式设置 */
    switch (currentSUMode) {
    
    case MyAppNS::PreviewMode:
        currentDUMode = MyAppNS::PreviousOnPreview;
        break;
        
    case MyAppNS::FullScreenMode: {
        currentDUMode = MyAppNS::PreviousOnFullScreen;
        
        /* 重置缩放系数、旋转角度 */
        currentRotationAngle = 0;
        currentScaleRatio = 1.0;
    }
        break;
    }
    
    /* 开启定时器 */
    dynamicRefreshComplete = false;
    dynamicUpdateTimer->start(50);
}

void MainWindow::slotClickedPbNext(void) {
    if (!dynamicRefreshComplete)
        return;
    
    /* 如果正在播放则停止播放 */
    if (playTimer->isActive()) {
        playTimer->stop();
        pbPlay->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton:hover {border-image: url(:/icon/Resource/play2)}"
                              "QPushButton:pressed {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                              );
    }
    
    /* 更新index */
    leftImageIndex = midImageIndex;
    midImageIndex = rightImageIndex;
    rightImageIndex = rightImageIndex + 1 < imageNum? rightImageIndex + 1: 0;
    
    /*更新模式设置 */
    switch (currentSUMode) {
    
    case MyAppNS::PreviewMode:
        currentDUMode = MyAppNS::NextOnPreview;
        break;
        
    case MyAppNS::FullScreenMode: {
        currentDUMode = MyAppNS::NextOnFullScreen;
        
        /* 重置缩放系数、旋转角度 */
        currentRotationAngle = 0;
        currentScaleRatio = 1.0;
    }
        break;
    }
    
    /* 开启定时器 */
    dynamicRefreshComplete = false;
    dynamicUpdateTimer->start(50);
}

void MainWindow::slotClickedPbEnlarge(void) {
    /* 这里只需要验证当前状态即可 不需要验证当前的模式 因为这个按钮只会在全屏模式下才会出现！ */
    if (!dynamicRefreshComplete)
        return;
    
    /* 如果正在播放则停止播放 */
    if (playTimer->isActive()) {
        playTimer->stop();
        pbPlay->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton:hover {border-image: url(:/icon/Resource/play2)}"
                              "QPushButton:pressed {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                              );
    }
    
    /* 更新模式 无需设置静态模式 此时肯定是全屏模式 */
    currentDUMode = MyAppNS::Scale;
    
    /* 修改缩放比例系数 */
    if (2.0 <= currentScaleRatio)
        return;
    currentScaleRatio += ONCE_SCALE_RATIO;
    
    /* 开启定时器 */
    dynamicUpdateTimer->start(50);
    dynamicRefreshComplete = false;
}

void MainWindow::slotClickedPbReduce(void) {
    /* 这里只需要验证当前状态即可 不需要验证当前的模式 因为这个按钮只会在全屏模式下才会出现！ */
    if (!dynamicRefreshComplete)
        return;
    
    /* 如果正在播放则停止播放 */
    if (playTimer->isActive()) {
        playTimer->stop();
        pbPlay->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton:hover {border-image: url(:/icon/Resource/play2)}"
                              "QPushButton:pressed {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                              );
    }
    
    /* 更新模式 无需设置静态模式 此时肯定是全屏模式 */
    currentDUMode = MyAppNS::Scale;
    
    /* 修改缩放比例系数 */
    if (0.4 >= currentScaleRatio)
        return;
    currentScaleRatio -= ONCE_SCALE_RATIO;
    
    /* 开启定时器 */
    dynamicUpdateTimer->start(50);
    dynamicRefreshComplete = false;
}

void MainWindow::slotClickedPbRN90(void) {
    if (!dynamicRefreshComplete)
        return;
    
    /* 如果正在播放则停止播放 */
    if (playTimer->isActive()) {
        playTimer->stop();
        pbPlay->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton:hover {border-image: url(:/icon/Resource/play2)}"
                              "QPushButton:pressed {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                              );
    }
    
    /* 更新模式设置 */
    currentDUMode = MyAppNS::RotateNegative90;
    
    /* 开启定时器 */
    dynamicUpdateTimer->start(50);
    dynamicRefreshComplete = false;
}

void MainWindow::slotClickedPbRP90(void) {
    if (!dynamicRefreshComplete)
        return;
    
    /* 如果正在播放则停止播放 */
    if (playTimer->isActive()) {
        playTimer->stop();
        pbPlay->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton:hover {border-image: url(:/icon/Resource/play2)}"
                              "QPushButton:pressed {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                              );
    }
    
    /* 更新模式设置 */
    currentDUMode = MyAppNS::RotatePositive90;
    
    /* 开启定时器 */
    dynamicUpdateTimer->start(50);
    dynamicRefreshComplete = false;
}



/*
 * 虚函数
 */
void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    QPixmap pixmap;
    
    if (!dynamicRefreshComplete) {
        
        switch (currentDUMode) {
        
        case MyAppNS::ListToFullScreen: {
            pixmap.load(imageArr[midImageIndex].filePath);
            painter.drawPixmap((float)(275 - dUTimerCount * 55)/800*this->geometry().width(),
                               (float)(165 - 33 * dUTimerCount)/480*this->geometry().height(),
                               (float)(250 + 110 * dUTimerCount)/800*this->geometry().width(),
                               (float)(150 + 66 * dUTimerCount)/480*this->geometry().height(),
                               pixmap);
        }
            break;
            
        case MyAppNS::ListToPreview: {
            pixmap.load(imageArr[leftImageIndex].filePath);
            painter.setOpacity(1.0 - (dUTimerCount * 0.1));
            painter.drawPixmap((float)(-400 + dUTimerCount * 80)/800*this->geometry().width(),
                               (float)120/480*this->geometry().height(), (float)400/800*this->geometry().width(),
                               (float)240/480*this->geometry().height(), pixmap);
            pixmap.load(imageArr[rightImageIndex].filePath);
            painter.drawPixmap((float)(800 - dUTimerCount * 80)/800*this->geometry().width(),
                               (float)120/480*this->geometry().height(), (float)400/800*this->geometry().width(),
                               (float)240/480*this->geometry().height(), pixmap);
            pixmap.load(imageArr[midImageIndex].filePath);
            painter.setOpacity(1.0);
            painter.drawPixmap((float)150/800*this->geometry().width(), (float)(0 + 18 * dUTimerCount)/480*this->geometry().height(),
                               (float)500/800*this->geometry().width(), (float)300/480*this->geometry().height(), pixmap);
        }
            break;
            
        case MyAppNS::PreviewToFullScreen: {
            pixmap.load(imageArr[midImageIndex].filePath);
            painter.drawPixmap((float)(150 - dUTimerCount * 30)/800*this->geometry().width(),
                               (float)(90 - 18 * dUTimerCount)/480*this->geometry().height(),
                               (float)(500 + 60 * dUTimerCount)/800*this->geometry().width(),
                               (float)(300 + 36 * dUTimerCount)/480*this->geometry().height(), pixmap);
        }
            break;
            
        case MyAppNS::FullScreenToPreview: {
            pixmap.load(imageArr[leftImageIndex].filePath);
            painter.setOpacity(1.0 - (dUTimerCount * 0.1));
            painter.drawPixmap((float)(-400 + dUTimerCount * 80)/800*this->geometry().width(),
                               (float)120/480*this->geometry().height(), (float)400/800*this->geometry().width(),
                               (float)240/480*this->geometry().height(), pixmap);
            pixmap.load(imageArr[rightImageIndex].filePath);
            painter.drawPixmap((float)(800 - dUTimerCount * 80)/800*this->geometry().width(),
                               (float)120/480*this->geometry().height(), (float)400/800*this->geometry().width(),
                               (float)240/480*this->geometry().height(), pixmap);
            pixmap.load(imageArr[midImageIndex].filePath);
            painter.setOpacity(1.0);
            painter.drawPixmap((float)(0 + 30 * dUTimerCount)/800*this->geometry().width(),
                               (float)(0 + 18 * dUTimerCount)/480*this->geometry().height(),
                               (float)(800 - 60 * dUTimerCount)/800*this->geometry().width(),
                               (float)(480 - 36 * dUTimerCount)/480*this->geometry().height(), pixmap);
        }
            break;
            
        case MyAppNS::PreviousOnFullScreen: {
            pixmap.load(imageArr[midImageIndex].filePath);
            painter.drawPixmap((float)(-800 + dUTimerCount * 160)/800*this->geometry().width(),
                               0, this->geometry().width(), this->geometry().height(), pixmap);
            
            pixmap.load(imageArr[rightImageIndex].filePath);
            painter.drawPixmap((float)(0 + dUTimerCount * 160)/800*this->geometry().width(),
                               0, this->geometry().width(), this->geometry().height(), pixmap);
        }
            break;
            
        case MyAppNS::NextOnFullScreen: {
            pixmap.load(imageArr[midImageIndex].filePath);
            painter.drawPixmap((float)(800 - dUTimerCount * 160)/800*this->geometry().width(),
                               0, this->geometry().width(), this->geometry().height(), pixmap);
            
            pixmap.load(imageArr[leftImageIndex].filePath);
            painter.drawPixmap((float)(0 - dUTimerCount * 160)/800*this->geometry().width(),
                               0, this->geometry().width(), this->geometry().height(), pixmap);
        }
            break;
            
        case MyAppNS::PreviousOnPreview: {
            pixmap.load(imageArr[leftImageIndex].filePath);
            painter.setOpacity(1.0 - (dUTimerCount * 0.1));
            
            /* 画左边的图像 */
            painter.drawPixmap((float)(150 - dUTimerCount * 30)/800*this->geometry().width(),
                               (float)120/480*this->geometry().height(), (float)400/800*this->geometry().width(),
                               (float)240/480*this->geometry().height(), pixmap);
            
            /* 画右边的图像 */
            pixmap.load(imageArr[rightImageIndex].filePath);
            painter.drawPixmap((float)(250 + 30 * dUTimerCount)/800*this->geometry().width(),
                               (float)(90 + 6 * dUTimerCount)/480*this->geometry().height(),
                               (float)(500 - 20 * dUTimerCount)/800*this->geometry().width(),
                               (float)(300 - 12 * dUTimerCount)/480*this->geometry().height(),
                               pixmap);
            
            /* 画中间的图像 */
            painter.setOpacity(0.5 + (dUTimerCount * 0.1));
            pixmap.load(imageArr[midImageIndex].filePath);
            painter.drawPixmap((float)(0 + 30 * dUTimerCount)/800*this->geometry().width(),
                               (float)(120 - 6 * dUTimerCount)/480*this->geometry().height(),
                               (float)(400 + 20 * dUTimerCount)/800*this->geometry().width(),
                               (float)(240 + 12 * dUTimerCount)/480*this->geometry().height(),
                               pixmap);
        }
            break;
            
        case MyAppNS::NextOnPreview: {
            pixmap.load(imageArr[rightImageIndex].filePath);
            painter.setOpacity(1.0 - (dUTimerCount * 0.1));
            
            /* 画右边的图像 */
            painter.drawPixmap((float)(250 + dUTimerCount * 30)/800*this->geometry().width(),
                               (float)120/480*this->geometry().height(), (float)400/800*this->geometry().width(),
                               (float)240/480*this->geometry().height(), pixmap);
            
            /* 画左边的图像 */
            pixmap.load(imageArr[leftImageIndex].filePath);
            painter.drawPixmap((float)(150 - 30 * dUTimerCount)/800*this->geometry().width(),
                               (float)(90 + 6 * dUTimerCount)/480*this->geometry().height(),
                               (float)(500 - 20 * dUTimerCount)/800*this->geometry().width(),
                               (float)(300 - 12 * dUTimerCount)/480*this->geometry().height(),
                               pixmap);
            
            /* 画中间的图像 */
            painter.setOpacity(0.5 + (dUTimerCount * 0.1));
            pixmap.load(imageArr[midImageIndex].filePath);
            painter.drawPixmap((float)(400 - 50 * dUTimerCount)/800*this->geometry().width(),
                               (float)(120 - 6 * dUTimerCount)/480*this->geometry().height(),
                               (float)(400 + 20 * dUTimerCount)/800*this->geometry().width(),
                               (float)(240 + 12 * dUTimerCount)/480*this->geometry().height(),
                               pixmap);
        }
            break;
            
        case MyAppNS::Scale: {
            /* 计算缩放 */
            pixmap.load(imageArr[midImageIndex].filePath);
            int temp = this->geometry().width() * currentScaleRatio - this->geometry().width();
            int width = -temp/2;
            temp  = this->geometry().height() * currentScaleRatio - this->geometry().height();
            int height = -temp/2;
            
            /* 计算旋转角度 */
            painter.save();
            painter.translate(this->geometry().width()/2, this->geometry().height()/2); //
            painter.rotate(currentRotationAngle);
            painter.translate(-(this->geometry().width()/2 - width), -(this->geometry().height()/2 - height));
            
            /* 绘制 */
            painter.drawPixmap(0, 0, this->geometry().width() * currentScaleRatio,
                               this->geometry().height() * currentScaleRatio, pixmap);
            painter.restore();
        }
            break;
            
        case MyAppNS::RotateNegative90: {
            pixmap.load(imageArr[midImageIndex].filePath);
            
            /* 计算缩放 */
            int temp = this->geometry().width() * currentScaleRatio - this->geometry().width();
            int width = -temp/2;
            temp  = this->geometry().height() * currentScaleRatio - this->geometry().height();
            int height = -temp/2;
            
            /* 计算旋转 */
            painter.save();
            painter.translate(this->geometry().width()/2, this->geometry().height()/2);
            painter.rotate(currentRotationAngle - 18 * dUTimerCount);
            painter.translate(-(this->geometry().width()/2 - width), -(this->geometry().height()/2 - height));
            painter.drawPixmap(0, 0, this->geometry().width() * currentScaleRatio,
                               this->geometry().height() * currentScaleRatio, pixmap);
            painter.restore();
        }
            break;
            
        case MyAppNS::RotatePositive90: {
            pixmap.load(imageArr[midImageIndex].filePath);
            
            /* 计算缩放 */
            int temp = this->geometry().width() * currentScaleRatio - this->geometry().width();
            int width = -temp/2;
            temp  = this->geometry().height() * currentScaleRatio - this->geometry().height();
            int height = -temp/2;
            
            /* 计算旋转 */
            painter.save();
            painter.translate(this->geometry().width()/2, this->geometry().height()/2);
            painter.rotate(currentRotationAngle + 18 * dUTimerCount);
            painter.translate(-(this->geometry().width()/2 - width), -(this->geometry().height()/2 - height));
            painter.drawPixmap(0, 0, this->geometry().width() * currentScaleRatio,
                               this->geometry().height() * currentScaleRatio, pixmap);
            painter.restore();
        }
            break;
            
        default: break;
        }
        
        if (5 == dUTimerCount) {
            dUTimerCount = 0;
            dynamicRefreshComplete = true;
            dynamicUpdateTimer->stop();
            if (MyAppNS::RotatePositive90 == currentDUMode)
                currentRotationAngle += 90.0;
            if (MyAppNS::RotateNegative90 == currentDUMode)
                currentRotationAngle -= 90.0;
        }
    }
    else {
        
        switch (currentSUMode) {
        
        case MyAppNS::ListMode:
            break;
            
        case MyAppNS::PreviewMode: {
            pixmap.load(imageArr[leftImageIndex].filePath);
            painter.setOpacity(0.5);
            painter.drawPixmap(0, (float)120/480*this->geometry().height(),
                               (float)400/800*this->geometry().width(),
                               (float)240/480*this->geometry().height(), pixmap);
            
            pixmap.load(imageArr[rightImageIndex].filePath);
            painter.drawPixmap((float)400/800*this->geometry().width(),
                               (float)120/480*this->geometry().height(),
                               (float)400/800*this->geometry().width(),
                               (float)240/480*this->geometry().height(), pixmap);
            
            pixmap.load(imageArr[midImageIndex].filePath);
            painter.setOpacity(1.0);
            painter.drawPixmap((float)150/800*this->geometry().width(),
                               (float)90/480*this->geometry().height(),
                               (float)500/800*this->geometry().width(),
                               (float)300/480*this->geometry().height(), pixmap);
        }
            break;
            
        case MyAppNS::FullScreenMode: {
            /* 计算缩放 */
            pixmap.load(imageArr[midImageIndex].filePath);
            int temp = this->geometry().width() * currentScaleRatio - this->geometry().width();
            int width = -temp/2;
            temp  = this->geometry().height() * currentScaleRatio - this->geometry().height();
            int height = -temp/2;
            
            /* 计算旋转角度 */
            painter.save();
            painter.translate(this->geometry().width()/2, this->geometry().height()/2); //
            painter.rotate(currentRotationAngle);
            painter.translate(-(this->geometry().width()/2 - width), -(this->geometry().height()/2 - height));
            
            /* 绘制 */
            painter.drawPixmap(0, 0, this->geometry().width() * currentScaleRatio,
                               this->geometry().height() * currentScaleRatio, pixmap);
            painter.restore();
        }
            break;
            
        default: break;
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    
    if (MyAppNS::ListMode == currentSUMode ||
            MyAppNS::PreviewMode == currentSUMode)
        return;
    
    if (hideMenuBarTimer->isActive())
        hideMenuBarTimer->stop();
    
    hideMenuBarTimer->start(1500);
    
    /* 如果菜单栏此时是可见的 在不必再进行任何操作了 */
    if ( bottomMenuBar->isVisible() )
        return;
    
    bottomMenuBar->setVisible(true);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    
    /* 如果是列表模式 则不支持这种方式 */
    if (MyAppNS::ListMode == currentSUMode ||
            !dynamicRefreshComplete)
        return;
    
    switch (currentSUMode) {
    
    case MyAppNS::PreviewMode: {
        currentDUMode = MyAppNS::PreviewToFullScreen;
        currentSUMode = MyAppNS::FullScreenMode;
        
        /* 控件位置配置 */
        setFullScreenModeButtonGeometry();
        displayButtonInFullScreenMode();
        bottomMenuBar->setStyleSheet("QWidget {background-color: rgba(50, 50, 50, 50%); }");
    }
        break;
        
    case MyAppNS::FullScreenMode: {
        currentDUMode = MyAppNS::FullScreenToPreview;
        currentSUMode = MyAppNS::PreviewMode;
        
        /* 关闭隐藏菜单栏的定时器 */
        hideMenuBarTimer->stop();
        
        /* 如果正在播放则停止播放 */
        if (playTimer->isActive()) {
            playTimer->stop();
            pbPlay->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/play1)}"
                                  "QPushButton:hover {border-image: url(:/icon/Resource/play2)}"
                                  "QPushButton:pressed {border-image: url(:/icon/Resource/play1)}"
                                  "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                                  );
        }
        
        /* 重置缩放系数、旋转角度 */
        currentRotationAngle = 0;
        currentScaleRatio = 1.0;
        
        /* 显示控件 */
        setPreviewModeButtonGeometry();
        displayButtonInPreviewMode();
        bottomMenuBar->setStyleSheet("QWidget {background-color: rgba(50, 50, 50, 100%); }");
    }
        break;
        
    default: break;
    }
    
    /* 开启定时器 */
    dynamicRefreshComplete = false;
    dynamicUpdateTimer->start(50);
}


void MainWindow::mousePressEvent(QMouseEvent *event) {
    mousePoint.setY(cursor().pos().y());// 记录按下点y坐标值
    if (MyAppNS::ListMode == currentSUMode)
        return;
    
    lastCoordinate->setX(event->pos().x());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    int previousPoint = mousePoint.y();     // 获取按下点y坐标值
    mousePoint.setY(cursor().pos().y());    // 获取释放点的y坐标值
    
    if(mousePoint.y() < (float)150/480*this->geometry().height())     //防止用户滑动大于150的区域而触发关闭按钮出现
    {
        int offset =   mousePoint.y() - previousPoint;
        //无
    }
    
    if (!dynamicRefreshComplete ||
            MyAppNS::ListMode == currentSUMode)
        return;
    
    int distance = event->pos().x() - lastCoordinate->x();
    if (100 > qAbs(distance))
        return;
    
    /* 如果正在播放则停止播放 */
    if (playTimer->isActive()) {
        playTimer->stop();
        pbPlay->setStyleSheet("QPushButton {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton:hover {border-image: url(:/icon/Resource/play2)}"
                              "QPushButton:pressed {border-image: url(:/icon/Resource/play1)}"
                              "QPushButton {background-color: rgba(255, 255, 255, 0%); outline: none}"
                              );
    }
    
    /* 更新index */
    if (0 > distance) {
        leftImageIndex = midImageIndex;
        midImageIndex = rightImageIndex;
        rightImageIndex = rightImageIndex + 1 < imageNum? rightImageIndex + 1: 0;
    }
    else {
        rightImageIndex = midImageIndex;
        midImageIndex = leftImageIndex;
        leftImageIndex = leftImageIndex - 1 >= 0? leftImageIndex - 1: imageNum - 1;
    }
    
    switch (currentSUMode) {
    
    case MyAppNS::PreviewMode: {
        /* 设置模式 */
        if (0 < distance)
            currentDUMode = MyAppNS::PreviousOnPreview;
        else
            currentDUMode = MyAppNS::NextOnPreview;
    }
        break;
        
    case MyAppNS::FullScreenMode: {
        /* 设置模式 */
        if (0 < distance)
            currentDUMode = MyAppNS::PreviousOnFullScreen;
        else
            currentDUMode = MyAppNS::NextOnFullScreen;
    }
        break;
        
    default: break;
    }
    
    /* 重置缩放系数、旋转角度 */
    currentRotationAngle = 0;
    currentScaleRatio = 1.0;
    
    /* 开启定时器 */
    dynamicRefreshComplete = false;
    dynamicUpdateTimer->start(50);
    
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QPushButton>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(0,0,QApplication::desktop()->screenGeometry().width(),
                      QApplication::desktop()->screenGeometry().height());
    //this->setGeometry(0,0,1366,768);

    QFont font;
    font.setPixelSize((float)13/800*this->geometry().width());

    this->setStyleSheet("QMainWindow{background-color:rgba(220,220,220,100%);}");
    this->setWindowFlags(Qt::FramelessWindowHint);
    lineEdit = new QLineEdit(this);
    lineEdit->setStyleSheet("QLineEdit{border-radius:3px}");//设置文本输入框的样式表
    lineEdit->setText("http://www.baidu.com");//默认网址为百度
    lineEdit->setMinimumHeight(30);
    if(this->geometry().width()==480)
        lineEdit->setMaximumWidth((float)150/800*this->geometry().width());
    else if(this->geometry().width()==800)
        lineEdit->setMaximumWidth((float)400/800*this->geometry().width());
    else if(this->geometry().width()==1024)
        lineEdit->setMaximumWidth((float)490/800*this->geometry().width());
    else if(this->geometry().width()==1280)
        lineEdit->setMaximumWidth((float)550/800*this->geometry().width());
    else if(this->geometry().width()==1366)
        lineEdit->setMaximumWidth((float)565/800*this->geometry().width());
    else if(this->geometry().width()==1920)
        lineEdit->setMaximumWidth((float)635/800*this->geometry().width());

    lineEdit->setFont(font);

    webView = new QWebView(this);
    page = new webPage(this);
    this->setCentralWidget(webView);
    webView->setPage(page);
    webView->installEventFilter(this);

    /* 添加动作图标与提示 */
    actionBack = new QAction(QIcon(":/icon/icon/back.png"),"后退到上一个网页ctrl+←",this);
    actionForward = new QAction(QIcon(":/icon/icon/forward.png"),"前进ctrl+→",this);
    actionLoadUrl = new QAction(QIcon(":/icon/icon/goto.png"),"转到Enter",this);
    actionClose = new QAction(QIcon(":/icon/icon/close.png"),"关闭ctrl+Q",this);
    actionZoomIn = new QAction(QIcon(":/icon/icon/zoom_in.png"),"放大ctrl+↑",this);
    actionZoomOut = new QAction(QIcon(":/icon/icon/zoom_out.png"),"缩小ctrl+↓",this);
    actionRefresh = new QAction(QIcon(":/icon/icon/refresh.png"),"刷新ctrl+R",this);
    actionHome = new QAction(QIcon(":/icon/icon/home.png"),"主页ctrl+H",this);

    actionZoomOut->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));
    actionZoomIn->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));
    actionClose->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    actionBack->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
    actionForward->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
    actionRefresh->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    actionHome->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
    actionLoadUrl->setShortcut(QKeySequence(Qt::Key_Return));

    /* 将lineEdit与actionLoadUrl添加到mainToolBar */
    ui->mainToolBar->addAction(actionBack);
    ui->mainToolBar->addAction(actionForward);
    ui->mainToolBar->addAction(actionRefresh);
    ui->mainToolBar->addAction(actionHome);
    ui->mainToolBar->addWidget(lineEdit);
    ui->mainToolBar->addAction(actionLoadUrl);
    ui->mainToolBar->addAction(actionClose);
    ui->mainToolBar->addAction(actionZoomIn);
    ui->mainToolBar->addAction(actionZoomOut);
    ui->mainToolBar->setMaximumHeight(36);
    ui->mainToolBar->setIconSize(QSize(36,36));
    ui->mainToolBar->setMovable(false);//禁止工具栏拖动，否则在嵌入式可能会有刷新不过来的情况出现


    connect(actionLoadUrl,SIGNAL(triggered()),this,SLOT(slotActionLoadUrlFunc()));
    connect(lineEdit,SIGNAL(returnPressed()),this,SLOT(slotActionLoadUrlFunc()));
    connect(page,SIGNAL(loadLink(QUrl)),this,SLOT(linkUrl(QUrl)));
    connect(page,SIGNAL(openLink(QUrl)),this,SLOT(openUrl(QUrl)));
    connect(actionBack,SIGNAL(triggered()),this,SLOT(slotUrlBack()));
    connect(actionForward,SIGNAL(triggered()),this,SLOT(slotUrlForward()));
    connect(actionClose,SIGNAL(triggered()),this,SLOT(slotClose()));
    connect(actionZoomIn,SIGNAL(triggered()),this,SLOT(slotZoomIn()));
    connect(actionZoomOut,SIGNAL(triggered()),this,SLOT(slotZoomOut()));
    connect(webView,SIGNAL(urlChanged(QUrl)),this,SLOT(slotUrlchanged()));
    connect(actionRefresh,SIGNAL(triggered()),this,SLOT(slotRefresh()));
    connect(actionHome,SIGNAL(triggered()),this,SLOT(slotHome()));

    timer = new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(slotTimertimeout()));

    ui->mainToolBar->installEventFilter(this);

    enterexitanimation = new Enterexitanimation(this);
    enterexitanimation->setGeometry(this->geometry().width()-48,0,48,48); //设置位置
    enterexitanimation->setAnimationObject(this);//设置动画对象
}

void MainWindow::slotActionLoadUrlFunc()
{
    /* 使用setUrl函数加载网址，单行输入框的内容直接输入 */
    webView->setUrl(QUrl(lineEdit->text()));
}

void MainWindow::linkUrl(QUrl)
{
    /* 处理不打开新窗口的连接 */
}
void MainWindow::openUrl(QUrl url)
{
    /* 处理新窗口的连接 */
    webView->setUrl(url);
}

void MainWindow::slotUrlBack()
{
    webView->back();
}

void MainWindow::slotUrlForward()
{
    webView->forward();
}

void MainWindow::slotClose()
{
    webView->close();
}

void MainWindow::slotZoomIn()
{
    qreal factor = webView->zoomFactor();
    webView->setZoomFactor(factor+0.1);
}

void MainWindow::slotZoomOut()
{
    qreal factor = webView->zoomFactor();
    webView->setZoomFactor(factor-0.1);
}

void MainWindow::slotUrlchanged()
{
    lineEdit->setText(webView->url().toString());
}

void MainWindow::slotRefresh()
{
    webView->reload();
}

void MainWindow::slotHome()
{
    webView->setUrl(QUrl("http://www.baidu.com"));
}

void MainWindow::slotTimertimeout()
{
    timer->stop();
    /* 开机先执行一次显示网页 */
    /* 这里延时1s才加载网页，因为出场动画与加载网页同时进行会感觉到卡 */
    slotActionLoadUrlFunc();
}
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    /* 定义事件过滤器，指定在webView上检测 */
    if (watched == webView && event->type() == QEvent::MouseButtonDblClick) {
        static bool mainToolHideFlag = true;
        if(mainToolHideFlag){
            ui->mainToolBar->hide();
            enterexitanimation->hide();
        }else{
            ui->mainToolBar->show();
            enterexitanimation->show();
        }
        mainToolHideFlag = !mainToolHideFlag;
    }
    return QWidget::eventFilter(watched,event);//将事件传递给父类
}

MainWindow::~MainWindow()
{
    webView->stop();
    delete ui;
}

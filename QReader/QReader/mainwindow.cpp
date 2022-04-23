#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWindowInit();
}

void MainWindow::mainWindowInit()
{
    //this->setWindowTitle("小说图书/阅读器");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(0,0,QApplication::desktop()->screenGeometry().width(),QApplication::desktop()->screenGeometry().height());

    //this->setGeometry(0,0,480,272);
    menu = new QMenuBar(this);
    menu->installEventFilter(this);
    //menu->setStyleSheet(MENU_STYLE);

    QFont font;
    font.setPixelSize(15);
    menu->setFont(font);

    fileMenu = new QMenu(this);
    helpMenu = new QMenu(this);
    readingMenu = new QMenu(this);

    fileMenu = menu->addMenu("文件");
    helpMenu = menu->addMenu("帮助");
    readingMenu = menu->addMenu("阅读模式");
    openAct = new QAction(QIcon(""),"打开",this);
    openAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    openAct->setStatusTip("打开");
    quitAct = new QAction(QIcon(""),"退出",this);
    quitAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    quitAct->setStatusTip("关闭");

    fileMenu->addAction(openAct);
    fileMenu->addAction(quitAct);

    aboutAct = new QAction(QIcon(""),"关于",this);
    aboutAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
    aboutAct->setStatusTip("关于");

    helpMenu->addAction(aboutAct);

    normalModelAct = new QAction(QIcon(""),"经典模式",this);
    normalModelAct->setStatusTip("经典模式");
    greenEyeModelAct = new QAction(QIcon(""),"绿色护眼",this);
    greenEyeModelAct->setStatusTip("绿色护眼");
    nightModelAct = new QAction(QIcon(""),"夜间模式",this);
    nightModelAct->setStatusTip("夜间模式");
    pergamynModelAct = new QAction(QIcon(""),"羊皮纸",this);
    pergamynModelAct->setStatusTip("羊皮纸");

    readingMenu->addAction(normalModelAct);
    readingMenu->addAction(greenEyeModelAct);
    readingMenu->addAction(nightModelAct);
    readingMenu->addAction(pergamynModelAct);

    /* 工具栏 */
    toolBar = new QToolBar(this);
    toolBar->setMovable(false);

    colorAct = new QAction(QIcon(":/icon/Resource/icon/A.png"),"字体颜色",this);
    fontZoomInAct = new QAction(QIcon(":/icon/Resource/icon/A+.png"),"字体增大",this);
    fontZoomOutAct = new QAction(QIcon(":/icon/Resource/icon/A-.png"),"字体减小",this);

    toolBar->addAction(colorAct);
    toolBar->addAction(fontZoomInAct);
    toolBar->addAction(fontZoomOutAct);
    this->toolBar->installEventFilter(this);//安装事件过滤器，用于监测事件，关闭整个窗口


    textBrowser = new QTextBrowser(this);
    //textBrowser->verticalScrollBar()->setStyleSheet(TEXTBROWSER_STYLE);
    /* 行号与行字数lable */
    textInfo = new QLabel(this);

    textInfo->setText("Ready");//设置默认显示内容


    menu->setGeometry(0,0,this->geometry().width(),30);
    toolBar->setGeometry(0,30,this->geometry().width(),30);
    textBrowser->setFont(QFont("宋体", 15));//设置默认字体：字体，字体大小
    textBrowser->setGeometry(0,60,this->geometry().width(),this->geometry().height()-80);//设置文本编辑区的大小与位置
    textBrowser->setTextInteractionFlags ( Qt::NoTextInteraction ); //禁止鼠标交互
    textInfo->setGeometry(this->geometry().width()-100,this->geometry().height()-20,100,20);
    textInfo->setAlignment(Qt::AlignCenter);

    vscroll = new Vscroll(this);
    vscroll->setObjectScrollBar(textBrowser->verticalScrollBar()); //指定优化的对象
    vscroll->setObjectList(textBrowser);//指定对象列表

    enterexitanimation = new Enterexitanimation(this);
    enterexitanimation->setGeometry(this->geometry().width()-48,0,48,48); //设置位置
    enterexitanimation->setAnimationObject(this);//设置动画对象

    connect(aboutAct,SIGNAL(triggered()),this,SLOT(slotactAbout()));
    connect(openAct,SIGNAL(triggered()),this,SLOT(slotOpen()));
    connect(quitAct,SIGNAL(triggered()),this,SLOT(slotQuit()));
    connect(normalModelAct,SIGNAL(triggered()),this,SLOT(slotactNormalReading()));
    connect(greenEyeModelAct,SIGNAL(triggered()),this,SLOT(slotactgreenEyeReading()));
    connect(nightModelAct,SIGNAL(triggered()),this,SLOT(slotactnightReading()));
    connect(pergamynModelAct,SIGNAL(triggered()),this,SLOT(slotactpergamynReading()));
    connect(textBrowser,SIGNAL(cursorPositionChanged()),this,SLOT(slotshowTextInfo()));
    connect(colorAct,SIGNAL(triggered()),this,SLOT(slotGetcolor()));
    connect(fontZoomInAct,SIGNAL(triggered()),this,SLOT(slotFontZoomIn()));
    connect(fontZoomOutAct,SIGNAL(triggered()),this,SLOT(slotFontZoomOut()));

    fileChose = new qFileChose(this);
}
void MainWindow::slotOpen()
{
    QString fileName;
    /* 这里重写了系统的默认打开文件对话框，因为系统打开文件对话框太大！，4.3寸 480*272屏不能完全显示，所以就重写了*/
    fileName = fileChose->choseOpenFileName(this,"选择文本文件", "./Resource/", "*.txt *.sh *.py *.cpp *.c",QFileDialog::DontUseNativeDialog|QFileDialog::ReadOnly);
    //fileName = QFileDialog::getOpenFileName(this, "选择文件", "./Resource/", "*.txt *.sh *.py *.cpp *.c", 0, QFileDialog::DontUseNativeDialog|QFileDialog::ReadOnly);
    if(!fileName.isEmpty()){
        loadFile(fileName);
    }

}

void MainWindow::slotQuit()
{
    qApp->quit();
}

void MainWindow::slotactAbout()
{
    QMessageBox box;
    box.about(this,"About","Author@Day");
    box.setFocusPolicy(Qt::NoFocus);
}

/* 加载文件的函数 */
void MainWindow::loadFile(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::critical(this,
                              "critical",
                              "cannot read file"
                              );
    }else{
        QTextStream in(&file);
        textBrowser->setText(in.readAll());
    }
}
/* 获取鼠标当前位置函数 */
void MainWindow::slotshowTextInfo()
{
    QTextCursor textCursor = textBrowser->textCursor();//获得鼠标光标
    int lineNum = textCursor.blockNumber();//行数
    int colNum = textCursor.columnNumber();//列数
    textInfo->setText(tr("行:%1,字:%2").arg(lineNum+1).arg(colNum));//显示在Label上，注意：行数是从0开始的
}

void MainWindow::slotGetcolor()
{
    QColor color = QColorDialog::getColor(Qt::red,this);
    if(color.isValid()){
        QTextCharFormat fmt;
        fmt.setForeground(color);
        QTextCursor textCursor = textBrowser->textCursor();//获取鼠标是否有选中文件
        if(textCursor.hasSelection()== false){
            textBrowser->selectAll();
        }
        textBrowser->mergeCurrentCharFormat(fmt);
        textCursor.clearSelection();
        textBrowser->setTextCursor(textCursor);
    }
}

void MainWindow::slotactNormalReading()
{
    textBrowser->setStyleSheet("background-color: rgba(255, 255, 255, 80%)");
}

void MainWindow::slotactgreenEyeReading()
{
    textBrowser->setStyleSheet("background-color: rgba(200, 250, 175, 80%)");
}

void MainWindow::slotactnightReading()
{
    textBrowser->setStyleSheet("background-color: rgba(200, 200, 200, 80%)");
}

void MainWindow::slotactpergamynReading()
{
    textBrowser->setStyleSheet("background-color: rgba(240, 240, 150, 80%)");
}

void MainWindow::slotFontZoomIn()
{
    textBrowser->zoomIn(1);
}

void MainWindow::slotFontZoomOut()
{
    textBrowser->zoomOut(1);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    //无
    return QWidget::eventFilter(watched,event);//将事件传递给父类
}

MainWindow::~MainWindow()
{
    delete ui;
}



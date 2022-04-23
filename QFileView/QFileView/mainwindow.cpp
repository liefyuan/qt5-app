#include "mainwindow.h"
#include "ui_mainwindow.h"
//文件夹浏览器，可以使用qt系统自带的文件管理模型来写，那样响应的速度会更快。
//本工程模拟成一个文件夹浏览的程序，非使用qt系统自带的文件管理模型。
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWindowInit();
    listWidgetInit();
    pushButttonInit();
    scandir();
    slotsInit();
    enterexitanimation = new Enterexitanimation(this);
    enterexitanimation->setGeometry(this->geometry().width()-48,0,48,48); //设置位置
    enterexitanimation->setAnimationObject(this);//设置动画对象
}

void MainWindow::mainWindowInit()
{
#ifdef __arm__
    USER_FILE_PATH ="/";              //用户要打开的目录
#else
    USER_FILE_PATH ="D:/";
#endif

    this->setGeometry(0,0,QApplication::desktop()->screenGeometry().width(),
                      QApplication::desktop()->screenGeometry().height());
    //this->setGeometry(0,0,480,272);
    QFont font;

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("QMainWindow{background-color: rgba(60, 60, 60, 80%);}");
    this->installEventFilter(this);

    font.setBold(true);
    font.setPixelSize((float)15/800*this->geometry().width());

    //其他初始化
    currtenPlaceLineEdit = new QLineEdit(this);
    currtenPlaceLineEdit->setGeometry((float)240/800*this->geometry().width(),(float)40/480*this->geometry().height(),
                                      (float)480/800*this->geometry().width(),(float)30/480*this->geometry().height());//195,15,605,30
    currtenPlaceLineEdit->setStyleSheet(CURRTENPLACELINEEDIT_STYLE);
    currtenPlaceLineEdit->setText(USER_FILE_PATH);
    currtenPlaceLineEdit->setFont(font);

    quickBars = new QLabel(this);
    quickBars->setGeometry((float)10/800*this->geometry().width(),(float)60/480*this->geometry().height(),
                           (float)100/800*this->geometry().width(),(float)30/480*this->geometry().height());//10,60,100,30
    quickBars->setText("Places");

    quickBars->setFont(font);
    quickBars->setStyleSheet("QLabel { color: white;}");

}

void MainWindow::slotsInit()
{
    connect(fileViewListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(fileViewListWidgetItemClicked(QListWidgetItem*)));
    connect(fileViewListWidget, SIGNAL(itemEntered(QListWidgetItem*)),
            this, SLOT(fileViewListWidgetItemEntered(QListWidgetItem*)));
    connect(fileViewListWidget, SIGNAL(customContextMenuRequested(const QPoint)),
            this, SLOT(fileViewListWidgetMouseRightButton(QPoint)));
    connect(pathListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(pathListWidgetItemCliked(QListWidgetItem*)));
    connect(upPushButton, SIGNAL(clicked()),this, SLOT(upPushButtonCliked()));
    connect(currtenPlaceLineEdit,SIGNAL(returnPressed()),this,SLOT(currtenPlaceLineEditReturnPressed()));
}

void MainWindow::listWidgetInit()
{
    QFont font;
    font.setPixelSize((float)12/800*this->geometry().width());
    fileViewListWidget = new QListWidget(this);
    fileViewListWidget->setMouseTracking(true);
    fileViewListWidget->setFont(font);
    fileViewListWidget->setContextMenuPolicy( Qt::CustomContextMenu);                //开启右键菜单功能
    fileViewListWidget->setMovement(QListWidget::Static);                            //开启不可拖动
    fileViewListWidget->setViewMode(QListView::IconMode);                            //设置为图标模式
    fileViewListWidget->setIconSize(QSize((float)72/800*this->geometry().width(),(float)72/800*this->geometry().width()));
    fileViewListWidget->setSpacing((float)35/800*this->geometry().width());          //设置间隔大小
    fileViewListWidget->setFrameShape(QListWidget::NoFrame);                         //隐藏外边框
    //fileViewListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);        //关闭垂直滚动条
    fileViewListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);        //关闭水平滚动条
    fileViewListWidget->setStyleSheet(FILEVIEWLISTWIDGET_STYLE);
    fileViewListWidget->verticalScrollBar()->setStyleSheet(FILEVIEWLISTWIDGET_VERTICAL_STYLE);
    fileViewListWidget->setGeometry((float)195/800*this->geometry().width(),(float)70/480*this->geometry().height(),
                                    (float)605/800*this->geometry().width(),(float)410/480*this->geometry().height());//195,50,605,430
    //fileViewListWidget->setFlow(QListView::LeftToRight);
    //fileViewListWidget->setProperty("isWrapping", QVariant(true));
    fileViewListWidget->setWordWrap(true);                                           //中文会自动换行，英文需要有空格才会自动换行

    pathListWidget     = new QListWidget(this);
    pathListWidget->setFrameShape(QListWidget::NoFrame);                                // 隐藏外边边框
    pathListWidget->setStyleSheet(PATHLISTWIDGET_STYLE);
    pathListWidget->setIconSize(QSize((float)20/800*this->geometry().width(),(float)20/800*this->geometry().width()));
    pathListWidget->setGeometry(0,(float)100/480*this->geometry().height(),
                                (float)195/800*this->geometry().width(),(float)120/480*this->geometry().height());//0,100,195,120
    pathListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pathListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    font.setPixelSize((float)15/800*this->geometry().width());
    pathListWidget->setFont(font);

    QString str = ":/icon/Resource/icon/";

    QListWidgetItem *item0 = new QListWidgetItem;
    item0->setText("Recent");
    item0->setIcon(QIcon(str + "Recent"));
    item0->setSizeHint(QSize((float)160/800*this->geometry().width(),
                             (float)30/480*this->geometry().height())
                       );
    pathListWidget->addItem(item0);

    QListWidgetItem *item1 = new QListWidgetItem;
    item1->setText("Home");
    item1->setIcon(QIcon(str + "Home"));
    item1->setSizeHint(QSize((float)195/800*this->geometry().width(),
                             (float)30/480*this->geometry().height())
                       );
    pathListWidget->addItem(item1);

    QListWidgetItem *item2 = new QListWidgetItem;
    item2->setText("/");
    item2->setSizeHint(QSize((float)195/800*this->geometry().width(),
                             (float)30/480*this->geometry().height())
                       );
    pathListWidget->addItem(item2);
    item2->setIcon(QIcon(str + "Gen"));

    vscroll = new Vscroll(this);
    vscroll->setObjectScrollBar(fileViewListWidget->verticalScrollBar()); //指定优化的对象
    vscroll->setObjectList(fileViewListWidget);//指定对象列表
}

void MainWindow::pushButttonInit()
{
    upPushButton      = new QPushButton(this);
    upPushButton->setFocusPolicy(Qt::NoFocus);
    upPushButton->setToolTip("返回上一层目录");
    upPushButton->setStyleSheet(UPPUSHBUTTON_STYLE_2);
    upPushButton->setGeometry((float)200/800*this->geometry().width(),(float)36/480*this->geometry().height(),
                              (float)35/800*this->geometry().width(),(float)35/800*this->geometry().width());

}


void MainWindow::fileViewListWidgetItemClicked(QListWidgetItem *item)
{
    QDir dir(arr[fileViewListWidget->row(item)].filePath);
    if (dir.exists()) {                                     //判断目录是存在，存在则"进入"并刷新当前列表
        USER_FILE_PATH = arr[fileViewListWidget->row(item)].filePath;
        upPushButton->setStyleSheet(UPPUSHBUTTON_STYLE_1);
        scandir();                                          //重新扫描“新目录”
        currtenPlaceLineEdit->setText(USER_FILE_PATH);
    }
}

void MainWindow::fileViewListWidgetItemEntered(QListWidgetItem *item)
{
    QDir dir(arr[fileViewListWidget->row(item)].filePath);
    //判断是不是目录，目录显示名称、路径、最后修改日期。文件显示名称和大小、路径、修改日期。
    //因为目录计算大小太耗时间，造成卡顿，这里提供计算目录的方法GetDirFileSize，但不计算目录的大小
    if(dir.exists()){
        item->setToolTip("目录名称："+arr[fileViewListWidget->row(item)].fileName +
                "\n文件夹路径："+ arr[fileViewListWidget->row(item)].filePath +
                "\n修改日期:"+arr[fileViewListWidget->row(item)].fileDate
                );
    }else{
        if( arr[fileViewListWidget->row(item)].fileSize == -1 ){//判断文件大小信息是否存在，防止重复计算这个文件的大小啦
            QFile file(arr[fileViewListWidget->row(item)].filePath);
            arr[fileViewListWidget->row(item)].fileSize = file.size();
        }
        item->setToolTip("文件名称："+arr[fileViewListWidget->row(item)].fileName +
                "\n文件路径："+ arr[fileViewListWidget->row(item)].filePath +
                "\n文件大小："+ QString::number(arr[fileViewListWidget->row(item)].fileSize) +
                "\n修改日期:"+arr[fileViewListWidget->row(item)].fileDate);
    }
}

void MainWindow::fileViewListWidgetMouseRightButton(const QPoint)
{
    QMenu *qMenu = new QMenu(this);
    QAction* refreshPaneAction = new QAction("&刷新",this);
    connect(refreshPaneAction, SIGNAL(triggered()), this, SLOT(scandir()));
    qMenu->addAction(refreshPaneAction);
    if(NULL != fileViewListWidget->currentItem() && fileViewListWidget->currentItem()->isSelected()){
        QAction* deletePaneAction = new QAction("删除文件/文件夹",this);
        qMenu->addAction(deletePaneAction);
        connect(deletePaneAction, SIGNAL(triggered()), this, SLOT(delectFileOrDir()));
    }

    qMenu->exec(QCursor::pos());
}

bool MainWindow::delectFileOrDir()//删除文件/文件夹方法
{
    QString files = arr[fileViewListWidget->currentRow()].filePath; //获取当前文件/文件夹的路径
    QString name = arr[fileViewListWidget->currentRow()].fileName;  //获取当前文件/文件夹的名称
    QFile file(files);                                              //文件对象
    QDir dir(files);                                                //目录对象
    QMessageBox::StandardButtons result;
    result = QMessageBox::warning(this,"警告","你是否想删除"+name+"?",
                                  QMessageBox::Yes | QMessageBox::Cancel);
    if(result == QMessageBox::Cancel){  //取消
        return false;
    }
    if(result == QMessageBox::Yes){     //Yes,表示删除
        if (dir.exists()){
            //dir.remove(files);
            deleteDirectory(files);     //删除文件夹及文件夹下面的所有内容
        }
        else{
            file.remove();              //删除文件
        }
        scandir();                      //删除后就再次刷新列表
        return true;
    }
    return result;
}

/* 参考网址https://www.jianshu.com/p/7a65d539cfef  修改了一些地方
 * QT默认没有删除空文件夹的方法，通过deleteDirectory来删除
 */
bool MainWindow::deleteDirectory(const QString &path)
{
    QDir dir(path);//传入目录路径
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);//过滤目录下面的文件，不要将..与.目录加进
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fi, fileList){
        if (fi.isFile()){
            fi.dir().remove(fi.fileName());
        }else{
            deleteDirectory(fi.absoluteFilePath()); //递归删除
        }
    }
    //return dir.rmpath(dir.absolutePath());        //使用这个会把上一级目录（如果为空目录）删除
    return dir.rmdir(path);
}

void MainWindow::pathListWidgetItemCliked(QListWidgetItem *item)
{
    switch (pathListWidget->row(item)) {
    case 1:
#ifdef __arm__
        USER_FILE_PATH = "/home/root";
#else
        USER_FILE_PATH = "F:/";
#endif
        break;
    case 2:
#ifdef __arm__
        USER_FILE_PATH = "/";
#else
        USER_FILE_PATH = "D:/";
#endif
        break;
    default:
        break;
    }
    if(pathListWidget->row(item) != 0){
        currtenPlaceLineEdit->setText(USER_FILE_PATH);
        upPushButton->setStyleSheet(UPPUSHBUTTON_STYLE_2);      //设置按钮样式为不可用状态
        scandir();                                              //重新扫描“新目录”
    }
}

void MainWindow::upPushButtonCliked()
{
    QDir dir(USER_FILE_PATH);
    if (dir.cdUp()){                                        //如果上一级目录存在则会返回真
        USER_FILE_PATH = dir.absolutePath();
        currtenPlaceLineEdit->setText(USER_FILE_PATH);
        scandir();
        if(!dir.cdUp()){                                    //判断再上一级是否存在
            upPushButton->setStyleSheet(UPPUSHBUTTON_STYLE_2);
        }
    }
}

/* 这里是参考网友https://blog.csdn.net/Best_ZYJ/article/details/81463167?utm_source=copy
 * 提供计算目录大小的方法，但程序未使用，因为太耗cpu、耗时间
 */
quint64 MainWindow::GetDirFileSize(const QString &dirPath)//获取目录的总大小
{
    QDir dir(dirPath);
    quint64 size = 0;
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files)){
        //计算文件大小
        size += fileInfo.size();
    }
    foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)){
        //若存在子目录，则递归调用dirFileSize()函数
        size += GetDirFileSize(dirPath + QDir::separator() + subDir);
    }
    return size;
}

void MainWindow::currtenPlaceLineEditReturnPressed()
{
    static QString currtenPlaceString;
    QDir dir(currtenPlaceLineEdit->text());
    //判断目录是否存在，检查当前目录是否改变。存在则"进入"并刷新当前列表
    if (dir.exists()) {
        if(currtenPlaceString != USER_FILE_PATH){
            USER_FILE_PATH = currtenPlaceLineEdit->text();
            scandir();
        }
    }else{
        QMessageBox *box = new QMessageBox();
        box->setText("输入的目录无效");
        box->show();
    }
    currtenPlaceString = currtenPlaceLineEdit->text();
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

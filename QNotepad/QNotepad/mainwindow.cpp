#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPushButton"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWindowInit();

}

void MainWindow::mainWindowInit()
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    menu = new QMenuBar(this);
    menu->installEventFilter(this);// 安装事件过滤器

    fileMenu = new QMenu(this);
    editMenu = new QMenu(this);
    helpMenu = new QMenu(this);

    fileMenu = menu->addMenu("文件");
    editMenu = menu->addMenu("编辑");
    helpMenu = menu->addMenu("帮助");

    newAct = new QAction(QIcon(""),"新建",this);
    newAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    newAct->setStatusTip("新建");
    openAct = new QAction(QIcon(""),"打开",this);
    openAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    openAct->setStatusTip("打开");
    saveAct = new QAction(QIcon(""),"保存",this);
    saveAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    saveAct->setStatusTip("保存");
    saveAsAct = new QAction(QIcon(""),"另存为",this);
    saveAsAct->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_S));
    saveAsAct->setStatusTip("另存为");
    quitAct = new QAction(QIcon(""),"退出",this);
    quitAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    quitAct->setStatusTip("退出");

    inputAct = new QAction(QIcon(""),"禁用输入法",this);
    menu->addAction(inputAct);



    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(quitAct);

    undoAct = new QAction(QIcon(""),"撤销",this);
    undoAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
    undoAct->setStatusTip("撤销");
    redoAct = new QAction(QIcon(""),"重做",this);
    redoAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
    redoAct->setStatusTip("重做");
    cutAct = new QAction(QIcon(""),"剪切",this);
    cutAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    cutAct->setStatusTip("剪切");
    copyAct = new QAction(QIcon(""),"复制",this);
    copyAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    copyAct->setStatusTip("复制");
    pasteAct = new QAction(QIcon(""),"粘贴",this);
    pasteAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    pasteAct->setStatusTip("粘贴");

    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);


    aboutAct = new QAction(QIcon(""),"关于",this);
    aboutAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));

    helpMenu->addAction(aboutAct);


    /* 文本编辑区 */
    textEdit = new QTextEdit(this);
    textEdit->setStyleSheet(TEXTEDIT_STYLE);

    /* 行号与行字数label */
    textInfo = new QLabel(this);
    textInfo->setText("Ready");
    textInfo->setAlignment(Qt::AlignCenter);

    this->setGeometry(0,0,QApplication::desktop()->screenGeometry().width(),
                      QApplication::desktop()->screenGeometry().height());
    //this->setGeometry(0,0,800,480);
    eInput = new easyInput(this);//输入法实例化
    eInput->installEventFilter(this);//安装事件过滤器
    inputFlag = true;

    textEdit->setFont(QFont("宋体", 15));//设置默认字体：字体，字体大小

    menu->setGeometry(0,0,this->geometry().width(),30);
    textInfo->setGeometry(this->geometry().width()-100,this->geometry().height()-20,100,20);

    //设置文本编辑区的大小与位置
    textEdit->setGeometry(0,30,
                          this->geometry().width(),
                          this->geometry().height()-50);
    oldHeigth = textEdit->geometry().height();

    connect(newAct,SIGNAL(triggered()),SLOT(slotNew()));
    connect(openAct,SIGNAL(triggered()),SLOT(slotOpen()));
    connect(saveAct,SIGNAL(triggered()),SLOT(slotSave()));
    connect(saveAsAct,SIGNAL(triggered()),SLOT(slotSaveAs()));
    connect(quitAct,SIGNAL(triggered()),SLOT(slotQuitApp()));
    connect(undoAct,SIGNAL(triggered()),this,SLOT(slotUndo()));
    connect(redoAct,SIGNAL(triggered()),this,SLOT(slotRedo()));
    connect(cutAct,SIGNAL(triggered()),this,SLOT(slotCut()));
    connect(copyAct,SIGNAL(triggered()),this,SLOT(slotCopy()));
    connect(pasteAct,SIGNAL(triggered()),this,SLOT(slotPaste()));
    connect(aboutAct,SIGNAL(triggered()),SLOT(slotactAbout()));
    connect(textEdit,SIGNAL(cursorPositionChanged()),this,SLOT(slotshowTextInfo()));
    connect(inputAct,SIGNAL(triggered()),SLOT(slotInputFlag()));

    vscroll = new Vscroll(this);
    vscroll->setObjectScrollBar(textEdit->verticalScrollBar()); //指定优化的对象
    vscroll->setObjectList(textEdit);//指定对象列表

    menu->setFocus(); // 界面初始化时，先把焦点在menu处，否则输入法会先出现
    fileChose = new qFileChose(this);

    enterexitanimation = new Enterexitanimation(this);
    enterexitanimation->setGeometry(this->geometry().width()-48,0,48,48); //设置位置
    enterexitanimation->setAnimationObject(this);//设置动画对象
}

void MainWindow::slotNew()
{
    if(maybeSave()){
        textEdit->clear();
        setFileName("");
    }
}

void MainWindow::slotOpen()
{
    if(maybeSave()){
        QString fileName;
        bool flag;//这时用这个bool类型的值来记录输入法是否打开的状态
        if(inputFlag == true){
            inputFlag = false;//false 为禁用输入法
            eInput->setEnabled(false);
            flag = true;
        }else{
            flag = false;
        }
        /* 这里重写了系统的默认打开文件对话框，因为系统打开文件对话框太大！，4.3寸 480*272屏不能完全显示，所以就重写了*/
        fileName = fileChose->choseOpenFileName(this,"选择文本文件", "./Resource/", "*.txt *.sh *.py *.cpp *.c",QFileDialog::DontUseNativeDialog|QFileDialog::ReadOnly);
        //fileName = QFileDialog::getOpenFileName(this, "选择文件", "./Resource/", "*.txt *.sh *.py *.cpp *.c", 0, QFileDialog::DontUseNativeDialog|QFileDialog::ReadOnly);
        if(!fileName.isEmpty()){
            loadFile(fileName);//这个文件必须是UTF-8的格式的，否则在开发板上会乱码
        }
        if(flag){
            inputFlag = true;
            eInput->setEnabled(true);
        }
    }
}

void MainWindow::slotQuitApp()
{
    if(maybeSave()){
        qApp->closeAllWindows();//关闭所有窗口
    }
}

void MainWindow::slotHideInput()
{
    eInput->hide();
}

void MainWindow::slotCut()
{
    textEdit->cut();
}

void MainWindow::slotCopy()
{
    textEdit->copy();
}

void MainWindow::slotPaste()
{
    textEdit->paste();
}

void MainWindow::slotRedo(){
    textEdit->redo();
}

void MainWindow::slotUndo()
{
    textEdit->undo();
}

void MainWindow::slotactAbout()
{
    QMessageBox::about(this,"About","参考草千里网友，@Day整理");
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    //    if(maybeSave()){
    //       e->accept();
    //    }else{
    //       e->ignore();//忽略，直接关闭
    //    }
}

/* 检查是否需要在新建或者退出前保存当前文件 */
bool MainWindow::maybeSave()
{
    if(textEdit->document()->isModified()){//判断文件是否被修改
        QMessageBox::StandardButtons result;
        result = QMessageBox::warning(this,"警告","退出前你想保存这个文件吗?",
                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if(result == QMessageBox::Cancel){//取消
            return false;
        }
        if(result == QMessageBox::Save){//保存
            return slotSave();
        }
        if(result == QMessageBox::Discard){//忽略，即不保存
            return true;
        }
    }else{
        return true;
    }
    //return result;
}

bool MainWindow::slotSave()
{
    if(currentFile.isEmpty()){
        return slotSaveAs();
    } else{
        return saveFile(currentFile);
    }
}

bool MainWindow::slotSaveAs()
{
    QString fileName =QFileDialog::getSaveFileName(this,tr("Save As"),
                                                   "/home/root/newFile",tr("Text Files (*.txt)"));
    if(fileName.isEmpty()){
        return false;
    }else{
        return saveFile(fileName);
    }
}

bool MainWindow::saveFile(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::critical(this,
                              "critical",
                              "cannot write file"
                              );
        return false;
    }else{
        QTextStream out(&file);
        out<<textEdit->toPlainText();
        setFileName(fileName);
        return true;
    }
}

void MainWindow::setFileName(QString fileName){
    currentFile = fileName;
    textEdit->document()->setModified(false);
    this->setWindowModified(false);
    fileName.isEmpty() ?
                this->setWindowFilePath("new.txt") :
                this->setWindowFilePath(fileName);
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
        textEdit->setText(in.readAll());
        setFileName(fileName);
    }
}
/* 获取鼠标当前位置函数 */
void MainWindow::slotshowTextInfo()
{
    QTextCursor textCursor = textEdit->textCursor();//获得鼠标光标
    int lineNum = textCursor.blockNumber();//行数
    int colNum = textCursor.columnNumber();//列数
    textInfo->setText(tr("行:%1,字:%2").arg(lineNum+1).arg(colNum));//显示在Label上，注意：行数是从0开始的

}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == menu){
        static QPoint point;
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            point.setY(cursor().pos().y());     // 记录按下点的y坐标
            eInput->hide();
            break;
        default:
            break;
        }
    }
    if(watched == eInput)
        switch (event->type()) {
        case QEvent::Hide:
            textEdit->resize(textEdit->geometry().width(),oldHeigth);
            break;
        case QEvent::Show:
        {
            if(inputFlag == false){
                eInput->hide();
                return false;
            }
            static int fitHeight = eInput->geometry().height();
            //            qDebug()<<fitHeight<<endl;
            textEdit->resize(textEdit->geometry().width(),oldHeigth-fitHeight+20);//重新设置文本编辑的区域大小
            textEdit->moveCursor(QTextCursor::End);//自动滑动到最后一行
        }
            break;
        default:
            break;
        }
    return QWidget::eventFilter(watched,event);//将事件传递给父类
}

void MainWindow::slotInputFlag()
{
    inputFlag = !inputFlag;
    if(!inputFlag){
        inputAct->setText("开启输入法");
        eInput->setDisabled(true);
        eInput->hide();
    }
    else{
        inputAct->setText("禁用输入法");
        eInput->setDisabled(false);
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}



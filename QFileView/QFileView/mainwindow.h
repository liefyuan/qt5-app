#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>

#include <QListWidget>
#include "config.h"
#include <QScrollBar>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QMenu>
#include <QTimer>
#include "vscroll.h"
#include "enterexitanimation.h"
namespace Ui {
class MainWindow;
}


//结构体用来存储遍历的文件信息
struct FileObjectInfo {
    QString fileName;                                               //文件名字
    QString filePath;                                               //文件路径
    QString fileDate;                                               //文件日期
    qint64  fileSize;                                               //文件大小
    QString fileSuffix;                                             //文件后缀
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void mainWindowInit();                                                  //主界面布局
    void listWidgetInit();                                                  //列表初始化
    void pushButttonInit();                                                 //按钮初始化
    void slotsInit();                                                       //信号槽初始化
    bool eventFilter(QObject *watched, QEvent *event);                      //事件过滤


private:
    Ui::MainWindow *ui;

    QString         USER_FILE_PATH;                                         //用户需要打开的目录路径
    QListWidget     *fileViewListWidget;                                    //文件/文件夹浏览列表
    QListWidget     *pathListWidget;                                        //路径列表
    QPushButton     *upPushButton;                                          //返回上一级目录按钮

    QVector         <struct FileObjectInfo> arr;                            //定义一个容器arr，用来存储遍历的文件信息

    QLineEdit       *currtenPlaceLineEdit;                                  //当前位置

    QLabel          *quickBars;

    quint64         GetDirFileSize(const QString &dirPath);                 //获取目录的总大小
    bool            deleteDirectory(const QString &path);                   //删除文件夹方法

    Vscroll         *vscroll;                                               //优化滚动条
    Enterexitanimation  *enterexitanimation;                                //进出场动画


private slots:
    void scandir();                                                     //扫描本地文件并添加至fileViewListWidget列表
    void fileViewListWidgetItemClicked(QListWidgetItem *item);          //单击列表中的item，如果是文件夹则进入，如果是文件则不做任何操作
    void fileViewListWidgetItemEntered(QListWidgetItem *item);          //当列表中的item选中时，提示这个文件的信息
    void fileViewListWidgetMouseRightButton(const QPoint);              //列表右键，功能处理函数
    void pathListWidgetItemCliked(QListWidgetItem *item);               //常用目录路径切换
    void upPushButtonCliked();                                          //返回一层目录按钮按下
    void currtenPlaceLineEditReturnPressed();                           //目录输入栏回车键按下
    bool delectFileOrDir();                                             //删除文件或者文件夹的方法
};

#endif // MAINWINDOW_H

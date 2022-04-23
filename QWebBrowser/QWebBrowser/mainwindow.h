#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QWebView>
#include <QLineEdit>
#include <QTimer>
#include "webpage.h"
#include "enterexitanimation.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWebView *webView;                  //定义QWebView对象
    QLineEdit  *lineEdit;               //网址输入框
    QAction *actionBack;                //返回
    QAction *actionForward;             //前进
    QAction *actionLoadUrl;             //跳转
    QAction *actionClose;               //关闭
    QAction *actionZoomIn;              //缩小
    QAction *actionZoomOut;             //放大
    QAction *actionRefresh;             //刷新
    QAction *actionHome;                //主页
    webPage *page;                      //重写的类，定义一个对象
    QTimer  *timer;                     //用于延时加载网页，因为6ull性能问题，出场动画需要延时几秒才流畅
    Enterexitanimation *enterexitanimation;

    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void slotActionLoadUrlFunc();       //加载网页
    void linkUrl(QUrl);                 //打不开的网页处理，TODO
    void openUrl(QUrl);                 //打开网页
    void slotUrlBack();                 //退回到上一个网页
    void slotUrlForward();              //前进
    void slotClose();                   //关闭
    void slotZoomIn();                  //缩小
    void slotZoomOut();                 //放大
    void slotUrlchanged();              //网址输入框替换
    void slotRefresh();                 //刷新
    void slotHome();                    //主页
    void slotTimertimeout();            //定时器时间到
};

#endif // MAINWINDOW_H

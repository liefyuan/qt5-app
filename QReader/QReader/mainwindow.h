#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"
#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <QTextEdit>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QCloseEvent>
#include <QTextBrowser>
#include <QColorDialog>
#include <QToolBar>
#include <QTimer>
#include <QScrollBar>
#include <QDesktopWidget>
#include "vscroll.h"
#include "enterexitanimation.h" //动画进场出场的类
#include "qfilechose.h"//重写的打开文件对话框
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

    void mainWindowInit();
    /* 菜单栏 */
    QMenuBar *menu;
    /* 3项菜单 */
    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *readingMenu;

    QAction *openAct;           //fileMenu的功能项，“打开”
    QAction *quitAct;           //fileMenu的动作，“退出”
    QAction *aboutAct;          //helpMenu的动作，关于
    //readingMenu的动作
    QAction *normalModelAct;    //经典模式
    QAction *greenEyeModelAct;  //绿色护眼
    QAction *nightModelAct;     //夜间模式
    QAction *pergamynModelAct;  //羊皮纸模式

    /* 工具栏 */
    QToolBar *toolBar;

    QAction *colorAct;          //颜色
    QAction *fontZoomInAct;     //字体增
    QAction *fontZoomOutAct;    //字体减


    /* 文本读取区 */
    QTextBrowser *textBrowser;
    QLabel *textInfo;

    Vscroll *vscroll;
    Enterexitanimation *enterexitanimation;
    qFileChose *fileChose;


    void loadFile(QString fileName);
    bool eventFilter(QObject *watched, QEvent *event);
private slots:
    void slotshowTextInfo();
    /* fileMenu的触发项 */
    void slotOpen();
    void slotQuit();
    void slotactAbout();
    void slotactNormalReading();
    void slotactgreenEyeReading();
    void slotactnightReading();
    void slotactpergamynReading();
    void slotFontZoomIn();
    void slotFontZoomOut();
    void slotGetcolor();
};

#endif // MAINWINDOW_H

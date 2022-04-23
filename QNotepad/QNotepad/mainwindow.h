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
#include <QScrollBar>
#include <QDesktopWidget>
#include "easyInput/easyinput.h"
#include "vscroll.h"
#include "qfilechose.h"
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

    void mainWindowInit();

    QMenuBar *menu;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    QAction *newAct;    //新建
    QAction *openAct;   //打开
    QAction *saveAct;   //保存
    QAction *saveAsAct; //另存为
    QAction *quitAct;   //退出

    QAction *undoAct;   //撤销
    QAction *redoAct;   //重做
    QAction *cutAct;    //剪切
    QAction *copyAct;   //复制
    QAction *pasteAct;  //粘贴
    QAction *inputAct;

    QAction *aboutAct;
    /* 文本编辑区 */
    QTextEdit *textEdit;
    QLabel *textInfo;

    QString currentFile;

    int     oldHeigth;  //用于保存原来高度
    easyInput *eInput;

    bool   inputFlag;   //是否开启输入法标志位
    Enterexitanimation *enterexitanimation;
    Vscroll *vscroll;

    qFileChose *fileChose;

    bool maybeSave();
    bool saveFile(QString fileName);
    void setFileName(QString fileName);
    void loadFile(QString fileName);

    void closeEvent(QCloseEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);
private slots:
    void slotshowTextInfo();

    void slotCut();
    void slotCopy();
    void slotPaste();
    void slotRedo();
    void slotUndo();
    void slotactAbout();

    void slotNew();
    bool slotSave();
    bool slotSaveAs();
    void slotOpen();
    void slotQuitApp();
    void slotHideInput();
public slots:
    void slotInputFlag();
};

#endif // MAINWINDOW_H

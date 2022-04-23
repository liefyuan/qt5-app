#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTextBrowser>
#include <QMainWindow>
#include <QProcess>
#include <QTextCodec>
#include <QDir>
#include <vscroll.h>
#include <config.h>
#include <QPushButton>
#include <enterexitanimation.h>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTimer          *timer;
    /* 测试网络 */
    QTextBrowser    *netTextBrowser;
    QProcess        *netProcess;
    Vscroll         *netVscroll;
    /* 测试ap3612 */
    QTextBrowser    *ap3216TextBrowser;
    QProcess        *ap3216Process;
    Vscroll         *ap3216Vscroll;
    /* 测试icm20608 */
    QTextBrowser    *icm20608TextBrowser;
    QProcess        *icm20608Process;
    Vscroll         *icm20608Vscroll;
    Enterexitanimation *enterexitanimation;

private:
    Ui::MainWindow *ui;
private slots:
    void np_readyReadStandardOutput();
    void tb_selectClear();
    void ap_readyReadStandardOutput();
    void ic_readyReadStandardOutput();
    void timerTimeOut();
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QProcess>
#include <QMainWindow>
#include <QTimer>
#include <enterexitanimation.h>
#include <rollingbox.h>
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
    QProcess *cameraProess;//用于开启摄像头进程
    RollingBox *rollingBox;//用于滚动选择分辨率
    Enterexitanimation *enterexitanimation;

    bool eventFilter(QObject *watched, QEvent *event);      //事件过滤器功能函数
    void startOrExitCamera();
private slots:
    void slot_cameraProessFinished(int);

};

#endif // MAINWINDOW_H

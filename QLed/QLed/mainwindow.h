#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"
#include <QMainWindow>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
#include <QGraphicsView>
#include <QProcess>
#include <QDebug>
#include <QPushButton>
#include <QTimer>
#include <QTime>
#include <QApplication>
#include <QDesktopWidget>
#include "enterexitanimation.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void execCmd1(int);
    void execCmd2(int);
    void timerTimeOut();

private:
    QButtonGroup *bgLed1;
    QButtonGroup *bgLed2;

    QLabel *labelLed1;
    QLabel *labelLed2;
    QLabel *labelTitle;

    QWidget *led1Widget;
    QWidget *led2Widget;

    QTimer *timer;

    bool flag1;
    bool flag2;

    QString imagePath;

    Enterexitanimation *enterexitanimation;

    void mainWindowInit();
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QLabel>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QTimer>
#include "enterexitanimation.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QTextCharFormat format;

    QCalendarWidget *calendar_widget;   // 日历结构体
    QWidget *parent_widget;             // 右边部分区域的父对象
    QLabel *today;                      // 今天
    QLabel *day;                        // 天
    QLabel *week;                       // 星期
    QLabel *year_month;                 // 年月
    QLabel *switch_year_month;
    QDate today_date;                   // 今天对应的日期

    QTimer *timer;                      // 定时器

    QPushButton *next_month;            // 日历显示下一个月
    QPushButton *prev_month;            // 日历显示上一个月

    Enterexitanimation *enterexitanimation;

    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void slot_timeout(void);
    void slot_currentPageChanged(int, int);
};

#endif // MAINWINDOW_H

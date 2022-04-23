#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "weater.h"
#include <QMainWindow>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextCodec>
#include <QFileInfo>

#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPushButton>

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
    void mainWindowInit();                          // 主窗体布局初始化
    void weatherDisplayInit();                      // 天气显示相关初始化
    void currentTimeInit();                         // 日期显示相关初始化
    void networkAccessManagerInit();                // QNetworkAccessManager初始化

private:
    Ui::MainWindow *ui;

    QWidget         *mainWidget;                    // 主Widget
    QWidget         *loopWidgetArr[5];              // 循环动画面板arr
    QWidget         *weatherWidgetArr[5];           // 天气显示面板背景arr，设置为透明
    QVBoxLayout     *vlayoutArr[5];                 // 水平布局arr
    QLabel          *weekLabelArr[5];               // 显示星期几arr
    QLabel          *minMaxTempLabelArr[5];         // 最高温度与最低温度arr
    QLabel          *weatherIconLabelArr[5];        // 天气图标标签arr
    QLabel          *weatherTypeLabelArr[5];        // 天气类型arr

    QGraphicsDropShadowEffect *loopGraphicsDropShadowEffect[5];
    QPropertyAnimation *propertyAnimation[5];
    QWidget  *topWidget;
    QLabel   *appNameLabel;
    QPushButton *exitPushButton;

    QDateTime       *currentTime;
    QLabel          *currentTimeLael;               // 当前时间
    QTimer          *updateTimer;                   // 更新时间定时器

    QNetworkAccessManager *NAManager;               // 网络请求

    QPushButton     *cityPushButton;                // 显示当前所在的地里位置（城市）
    QLabel          *currentTempLabel;              // 当前温度
    QLabel          *currentWeatherIcon;            // 当前天气类型图片
    QLabel          *currentWeather;                // 当前天气类型
    Enterexitanimation *enterexitanimation;         // 进出场动画
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void slotFinishedIpGetReply();
    void slotFinishedIpQueryReply();
    void slotFinishedWeatherGetReply();

    void updateTimerTimeOut();
    void backwardPushButtonClicked();
    void forwardPushButtonClicked();

};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QTextCodec>
#include <QFileInfo>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPushButton>
#include <QSlider>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDir>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "textticker.h"
#include "weather.h"
#include "config.h"
namespace Ui {
class MainWindow;
}

struct MediaObjectInfo {
    QString fileName;
    QString filePath;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void mainWindowInit();                          // 主窗体布局初始化
    void animationInit();                           // 天气显示相关初始化
    void mediaPlayerInit();                         // 媒体相关初始化
    void currentTimeInit();                         // 日期显示相关初始化
    QString getSystemSpeakerVolume();

private:
    Ui::MainWindow *ui;

    QWidget         *mainWidget;                    // 主Widget
    QWidget         *appWidgetParent;               // 存放app的父窗口
    QWidget         *appWidget;                     // 存放app的区域
    QWidget         *loopWidgetArr[17];             // 循环动画面板arr
    QWidget         *rightWidget;                   // 右边隐藏栏
    QPushButton     *iconPushButton[17];            // app图标按钮

    QWidget         *musicPlate;                    // 音乐板块
    QWidget         *weatherPlate;                  // 天气板块
    QWidget         *pageWidget[4];                 // 页面小圆点
    QLabel          *appNameLabel[17];              // app名字标签
    QLabel          *appMainInterface;              // app主界面label
    QPushButton     *speakersPushbutton;            // 扬声器图标
    int loopCount;                                  // 循环次数
    bool songsPlaySliderPressFlag;                  // 用于判断进度条是否被按压
    QLabel          *labelDuration;                 // 播放进度标签
    QSlider         *speakersSlider;                // 系统扬声器音量
    QSlider         *musicSlider;                   // 桌面音乐进度条
    QStringList     appPathStrList;                 // app路径定义
    QTimer          *appExecTimer;                  // 用于进入程序后过滤误触的点
    bool            flagAppExec;                    // 过滤标志位
    bool            scrollingFlag;                  // 判断app区域是否在移动

    QPropertyAnimation *propertyAnimation;

    QDateTime       *currentTime;
    QLabel          *currentTimeLael;               // 当前时间
    QLabel          *currentTimeLael2;              // 当前时间2
    QLabel          *currentTimeLael3;              // 当前时间3
    QLabel          *currentTimeLael4;              // 天气上次刷新的时间
    QTimer          *updateTimer;                   // 更新时间定时器
    QLabel          *musicImage;                    // 音乐旋转图片
    QLabel          *vloumeLabel;                   // 音量Label

    /* 媒体播放器相关 */
    QPushButton     *playPushButton;                //播放按钮
    QPushButton     *previousPushButton;            //上一首按钮
    QPushButton     *nextPushButton;                //下一首按钮
    QMediaPlayer    *mediaPlayer;                   //媒体播放器
    QMediaPlaylist  *mediaPlayList;                 //媒体列表
    QVector<MediaObjectInfo> mediaObjectInfo;       //媒体信息存储
    TextTicker      *songsName;                     //歌曲名称
    QString         strDuration;                    //媒体总长度


    QLabel          *messageLabel;                  //用于提示当前路径下有没有这个app

    /* 天气相关 */
    QLabel *weatherIcon;           //  天气图标
    QLabel *displayWeather;        //  显示天气
    QLabel *displayCity;           //  显示城市
    QLabel *displayTemp;           //  显示温度
    QLabel *displayMinMaxTemp;     //  显示当天的最大温度和最小温度

    /* 网络访问请求 */
    QNetworkAccessManager *NAManager;
    QTimer *timer;
    QTimer *musicTimer;            //  用于控制旋转
    QImage sourceImage;




    /* 私有方法 */
    void scanSongs();
    bool eventFilter(QObject *watched, QEvent *event);
    void networkAccessManagerInit(void);
    void weatherDisInit(void);
    void updateWeather(void);

    void painter();

public slots:
    void updateTimerTimeOut();
    void backward();
    void forward();
    void slotSetSpeakerVolume();
    void slotVolumeSpeakers();
    void slotappExecTimertimeout();
    void slotPlayPushButtonClicked();
    void slotPreviousPushButtonClicked();
    void slotnextPushButtonClicked();
    void mediaPlayerStateChanged(QMediaPlayer::State state);
    void mediaPlayerMediaChange(QMediaContent);


    void mediaPlayerPositionChanged(qint64 position);
    void mediaPlayerDurationChanged(qint64 duration);
    void songsPlaySliderPressed();
    void songsPlaySliderReleased();

    void slotFinishedIpGetReply(void);
    void slotFinishedIpQueryReply(void);
    void slotFinishedWeatherGetReply(void);
    void slotFinishedTimeGetReply(void);
};

#endif // MAINWINDOW_H

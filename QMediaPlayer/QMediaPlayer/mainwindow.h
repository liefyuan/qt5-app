#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QDir>
#include <QTextCodec>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QEvent>
#include <QMouseEvent>
#include <QVideoWidget>
#include <QFileDialog>
#include "config.h"
#include "enterexitanimation.h"
#include "qfilechose.h"
namespace Ui {
class MainWindow;
}

struct MediaObjectInfo {
    QString fileName;
    QString filePath;
    QString fileSuffix;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void mainWindowInit();                                  //主窗口初始化
    void scanMedia();                                       //扫描媒体
    void timerInit();                                       //定时器相关初始化
    void mediaPlayerInit();                                 //媒体播放器相关初始化
    void mediaListInit();                                   //媒体列表初始化
    void pushButtonInit();                                  //按钮相关初始化
    void labelInit();                                       //label标签相关初始化
    void sliderInit();                                      //进度条相关初始化
    void slotsInit();                                       //信号槽相关初始化
    bool eventFilter(QObject *watched, QEvent *event);      //事件过滤器功能函数
    QString findPhotoFunc();

private:
    Ui::MainWindow *ui;

    QWidget         *toolWidgetPanel;                       //用作一个底部播放面板
    QWidget         *mainWidget;                            //主Widget，用于存放背景

    QTimer          *timerVolumeSlider;                     //用来定时音量条是否可见
    QTimer          *timerPanelShow;                        //底板面板显示定时器

    QListWidget     *mediaList;                             //媒体列表

    QPushButton     *previousPushButton;                    //切换上一首
    QPushButton     *nextPushButton;                        //切换下一首
    QPushButton     *playOrPausePushButton;                 //播放/暂停
    QPushButton     *mutePushButton;                        //静音按钮
    QPushButton     *openFilePushButton;                    //打开文件按钮

    QMediaPlayer    *mediaPlayer;                           //媒体播放器
    QMediaPlaylist  *mediaPlayList;                         //媒体列表
    QVideoWidget    *playerWindow;                          //视频播放窗口

    QSlider         *mediaSlider;                           //媒体播放进度条
    QSlider         *volumeslider;                          //音量条

    QVector<MediaObjectInfo> mediaObjectInfo;               //媒体信息存储


    QString         strDuration;                            //媒体总长度
    int             previousPlaylistIndex;                  //用来记录当前正在播放（或已停止播放）的上一首媒体的index
    bool            mediaSliderPressFlag;                   //判断媒体进度条滑块是否按压
    qint64          errorPsition;                           //出错位置

    QLabel          *labelDuration;                         //播放进度标签
    QLabel          *labelmediasTitle;                      //媒体标题
    QLabel          *musicLabel;                            //媒体标题"音乐 请欣赏"

    Enterexitanimation *enterexitanimation;                 //进出场类动画
    qFileChose      *fileChose;

private slots:
    void mediaListDoubleCliked(QListWidgetItem*);           //双击媒体列表

    void mediaPlayerPositionChanged(qint64);                //媒体播放位置改变
    void mediaPlayerDurationChanged(qint64);                //媒体总长度改变
    void mediaPlayerCurrentMediaChanged(QMediaContent);     //媒体改变

    void playOrPausePushButtonCliked();                     //单击播放/暂停按钮
    void previousPushButtonCliked();                        //前一曲
    void nextPushButtonCliked();                            //下一曲
    void openfileButtonCilked();
    void mutePushButtonCliked();                            //静音

    void mediaPlayListCurrentIndexChanged(int);             //播放列表Index改变
    void mediaPlayerStateChanged(QMediaPlayer::State);      //媒体状态改变
    void mediaPlayerError(QMediaPlayer::Error);             //媒体错误处理
    void mediaPlayerStatusChanged(QMediaPlayer::MediaStatus);//媒体状态改变

    void mediaSliderPressed();                              //按下媒体进度条
    void mediaSliderReleased();                             //释放媒体进度条

    void volumeSliderValueChanged(int);                     //音量条值改变

    void timerVolumeSliderTimeOut();                        //定时器处理功能函数
    void timerPanelShowTimeOut();                           //播放面板定时出现
    void enterAnimation();

};

#endif // MAINWINDOW_H

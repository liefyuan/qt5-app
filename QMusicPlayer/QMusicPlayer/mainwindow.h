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
#include "config.h"
#include "enterexitanimation.h"
namespace Ui {
class MainWindow;
}

struct MediaObjectInfo {
    QString fileName;
    QString filePath;
};

//歌词结构体
typedef struct LrcParseFormat {
    QString time;
    QString content;
}LrcFormat;

typedef struct PlaybackMode {
    QString name;
    QString icon1;
    QString icon2;
}PbMode;

typedef struct SettingsMode {
    QString name;
    QString icon1;
    QString icon2;
}setMode;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void mainWindowInit();                                  //主窗口初始化
    void timerInit();                                       //定时器相关初始化
    void iconInit();                                        //图标初始化
    void mediaPlayerInit();                                 //媒体播放器相关初始化
    void songsListInit();                                   //歌曲列表初始化
    void playBackModeListInit();                            //播放模式列表初始化
    void settingsListInit();                                //设置列表初始化
    void scanSongs();                                       //扫描本地歌曲  备注：童鞋们，这个音乐播放器是扫描本地目录下的歌曲哦
    void pushButtonInit();                                  //按钮相关初始化
    void labelInit();                                       //label标签相关初始化
    void sliderInit();                                      //进度条相关初始化
    void slotsInit();                                       //信号槽相关初始化
    bool eventFilter(QObject *watched, QEvent *event);      //事件过滤器功能函数
    QString findPhotoFunc();

    Enterexitanimation *enterexitanimation;

private:
    Ui::MainWindow *ui;

    QTimer          *timerVolumeSlider;                     //用来定时音量条是否可见
    QTimer          *timerSongList;                         //用来启动后，列表缓慢出现

    QListWidget     *songList;                              //歌曲列表
    QListWidget     *playBackModeList;                      //播放模式选择
    QListWidget     *settingsList;                          //设置列表，此处隐藏在歌词中间部分右则，当鼠标移动到这里时出现

    QPushButton     *previousPushButton;                    //切换上一首
    QPushButton     *nextPushButton;                        //切换下一首
    QPushButton     *playOrPausePushButton;                 //播放/暂停
    QPushButton     *playbackMode;                          //播放模式
    QPushButton     *mutePushButton;                        //静音按钮

    QMediaPlayer    *mediaPlayer;                           //媒体播放器
    QMediaPlaylist  *mediaPlayList;                         //媒体列表

    QVector<LrcFormat> lrcArr;                              //lrc歌词数组
    QVector<PbMode>    pbModeArr;                           //播放模式图标数组

    QVector<setMode>   setModeArr;                          //设置模式相关项设置

    QSlider         *songsPlaySlider;                       //歌曲播放进度条
    QSlider         *volumeslider;                          //音量条

    QVector<MediaObjectInfo> mediaObjectInfo;               //媒体信息存储


    QString         strDuration;                            //媒体总长度
    int             previousPlaylistIndex;                  //用来记录当前正在播放（或已停止播放）的上一首歌曲的index
    bool            songsPlaySliderPressFlag;               //判断歌曲进度条滑块是否按压

    QLabel          *labelDuration;                         //播放进度标签
    QLabel          *labelSongsTitle;                       //歌曲标题
    QLabel          *LabelMainWindowBackground;             //用于覆盖桌面背景，防止桌面背景太亮

    QVector<QLabel *> labelArr;                             //歌词显示标签
private slots:
    void songListCliked(QListWidgetItem*);            //双击歌曲列表
    void playbackModeListItemEnter(QListWidgetItem *);  //当鼠标进入播放模式列表Item
    void playbackModeListItemCliked(QListWidgetItem *); //播放模式列表单击时
    void playbackModeListCurrentItemChange(QListWidgetItem*,QListWidgetItem*); //播放模式列表改变时
    void settingsListItemCliked(QListWidgetItem *);     //播放模式列表单击时

    void mediaPlayerPositionChanged(qint64);                //媒体播放位置改变
    void mediaPlayerDurationChanged(qint64);                //媒体总长度改变
    void mediaPlayerCurrentMediaChanged(QMediaContent);     //媒体改变

    void playOrPausePushButtonCliked();                     //单击播放/暂停按钮
    void previousPushButtonCliked();                        //前一曲
    void nextPushButtonCliked();                            //下一曲
    void playbackModeCliked();                              //播放模式
    void mutePushButtonCliked();                            //静音

    void mediaPlayListCurrentIndexChanged(int);             //播放列表Index改变
    void mediaPlayerStateChanged(QMediaPlayer::State);      //媒体状态改变
    void mediaPlayerError(QMediaPlayer::Error);             //媒体错误处理
    void mediaPlayerStatusChanged(QMediaPlayer::MediaStatus);//媒体状态改变

    void songsPlaySliderPressed();                          //按下歌曲进度条
    void songsPlaySliderReleased();                         //释放歌曲进度条

    void volumeSliderValueChanged(int);                     //音量条值改变

    void timerVolumeSliderTimeOut();                        //定时器处理功能函数
    void timerSongListTimeOut();

};

#endif // MAINWINDOW_H

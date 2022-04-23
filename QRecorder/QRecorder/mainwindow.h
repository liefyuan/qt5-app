#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QRadioButton>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMovie>
#include "config.h"
#include <QDial>
#include <QTimer>
#include <QProcess>
#include <QGraphicsDropShadowEffect>
#include <QFrame>
#include <QDir>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "enterexitanimation.h"
namespace Ui {
class MainWindow;
}

struct MediaObjectInfo{
    QString fileName;                              // 文件名
    QString filePath;                              // 文件路径
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTimer          *timerRecorder;                         // 定时器，用于计算录音时长
    QTimer          *timerVolumeSlider;                     // 定时器，当鼠标离开volumeslider后启动定时器1s后隐藏音量条
    QTimer          *timer;                                 // 延时，避免动画与QProcess造成卡顿

    QWidget         *mainWidget;                            // 主widget，用于布置整体背景
    QFrame          *verticalLine1;                         // 垂直美化"分割线一"
    QFrame          *verticalLine2;                         // 垂直美化"分割线二"

    QPushButton     *recorderButton;                        // 录音按钮
    QRadioButton    *micInDevice;                           // 录音设备一，micin
    QRadioButton    *lineInDevice;                          // 录音设备二，lineIn
    QPushButton     *previousPushButton;                    // 切换上一首
    QPushButton     *nextPushButton;                        // 切换下一首
    QPushButton     *playOrPausePushButton;                 // 播放/暂停
    QPushButton     *mutePushButton;                        // 静音按钮
    QPushButton     *deletePushButton;                      // 删除按钮

    QSlider         *mediaSlider;                           // 媒体播放进度条
    QSlider         *volumeslider;                          // 音量条
    QLabel          *labelDuration;                         // 媒体总长度与播放进度
    QLabel          *labelmediaTitle;                       // 当前媒体播放标题名称
    QLabel          *labelRecorder;                         // 显示录音时长
    QString         strDuration;                            // 媒体总长度
    int             previousPlaylistIndex;                  // 用于记录前一首的Index
    bool            mediaSliderPressFlag;                   // 是否按压
    int             recorderDuration;                       // 记录录音时长

    QListWidget     *recorderListWidget;                    // 录音列表

    QGraphicsDropShadowEffect *graphicsDropShadowEffect;    // 阴影特效，用于录音面板

    QProcess        *processCmd;                            // 进程，用于执行命令

    /* 媒体相关 */
    QMediaPlaylist  *mediaplaylist;                         // 媒体列表
    QMediaPlayer    *mediaplayer;                           // 媒体播放器
    QVector <MediaObjectInfo> mediaObjectInfo;              // 媒体信息容器

    Enterexitanimation *enterexitanimation;                 // 进出场动画与按钮

    void mainWindowInit();                                  // 主体布局初始化
    void pushButtonInit();                                  // 按钮初始化
    void listWidgetInit();                                  // 录音列表初始化
    void sliderInit();                                      // 进度条初始化
    void mediaplayerInit();                                 // 媒体初始化
    void labelInit();
    void scanAudio();                                       // 扫描本地媒体文体添加至列表
    void processInit();                                     // 进程初始化
    void slotsInit();
    bool eventFilter(QObject *watched, QEvent *event);      // 监测事件，处理事件
    void timerInit();


private slots:
    void recorderListWidgetDoubleCliked(QListWidgetItem*);  // 双击播放列表
    void recorderListWidgetCurrentRowChanged(int);          // 录音列表当前项变化时

    void mediaplayerPositionChanged(qint64);                // 媒体位置改变
    void mediaplayerDurationChanged(qint64);                // 媒体总长度改变
    void mediaplayerStateChanged(QMediaPlayer::State);      // 媒体状态改变时
    void mediaplaylistCurrentIndexChanged(int);             // 媒体列表改变时
    void mediaplayerCurrentMediaChanged(QMediaContent);     // 当前媒体变化时

    void playOrPausePushButtonCliked();                     // 单击播放或暂停按钮
    void nextPushButtonCliked();                            // 下一首按下时
    void previousPushButtonCliked();                        // 上一首按下时
    void mutePushButtonCliked();                            // 当静音按钮按下时
    void recorderButtonCliked();                            // 当录音按钮按下时

    void mediaSliderPressed();                              // 媒体滑条按下时
    void mediaSliderReleased();                             // 媒体滑条释放时

    void volumeSliderValueChanged(int);                     // 当媒体的音量值改变时

    void timerVolumeSliderTimeOut();                        // 定时1s隐藏音量条
    void timerRecorderTimeOut();                            // 录音时长记录
    void timerTimeOut();                                    // 延时处理process,避免动画冲突造成卡顿

    void deleteFile();                                      // 删除音频文件
    void addAudiofile(QString fileName);                    // 录音完成后，添加音频文件至列表中

    void switchRecorderDevice();                            // 切换录音设备处理

};

#endif // MAINWINDOW_H

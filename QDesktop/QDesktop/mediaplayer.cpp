#include "mainwindow.h"

void MainWindow::mediaPlayerInit()
{
    mediaPlayer           = new QMediaPlayer(this);
    mediaPlayList         = new QMediaPlaylist(this);
    mediaPlayList->clear();

    mediaPlayer->setPlaylist(mediaPlayList);                //设置媒体播放器的列表为mediaPlayList
    mediaPlayer->setNotifyInterval(1000);                   //设置触发时间长度

    mediaObjectInfo.clear();                                //清空
    connect(mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(mediaPlayerStateChanged(QMediaPlayer::State)));
    connect(mediaPlayer, SIGNAL(currentMediaChanged(QMediaContent)), this, SLOT(mediaPlayerMediaChange(QMediaContent)));

    connect(mediaPlayer,SIGNAL(positionChanged(qint64)),this,SLOT(mediaPlayerPositionChanged(qint64)));
    connect(mediaPlayer,SIGNAL(durationChanged(qint64)),this,SLOT(mediaPlayerDurationChanged(qint64)));
    connect(musicSlider,SIGNAL(sliderPressed()),this,SLOT(songsPlaySliderPressed()));
    connect(musicSlider,SIGNAL(sliderReleased()),this,SLOT(songsPlaySliderReleased()));
//音量与音量条设置
#ifndef __arm__
    mediaPlayer->setVolume(50);
    speakersSlider->setValue(50);
#endif
}
void MainWindow::scanSongs()
{
    QDir dir1(APP_CONFIG_AUDIO_FILE_PATH);
    //绝对路径转换，如果不转换则linux底层的播放器会找不到文件
    QDir dir2(dir1.absolutePath());
    if (dir2.exists()) {                                                    // 如果目录存在
        QStringList filter;                                                 // 定义过滤器
        filter << "*.mp3";                                                  // 包含所有.mp3后缀的文件
        QFileInfoList files = dir2.entryInfoList(filter, QDir::Files);      // 获取该目录下的所有文件
        for (int i = 0; i < files.count(); i++) {// 遍历
            MediaObjectInfo info;
            // 使用utf-8编码
            info.fileName = QString::fromUtf8(files.at(i).fileName().replace(".mp3", "").toUtf8().data());
            info.filePath = QString::fromUtf8(files.at(i).filePath().toUtf8().data());
            if (mediaPlayList->addMedia(QUrl::fromLocalFile(info.filePath)))//媒体列表添加歌曲
                mediaObjectInfo.append(info);                               //添加到容器数组里储存
            else {
                qDebug()<< mediaPlayList->errorString().toUtf8().data() << endl;
                qDebug() << "  Error number:" << mediaPlayList->error() << endl;
                continue;
            }
        }
    }
    else
        //目录如果不存在则创建此目录
        dir1.mkdir(APP_CONFIG_AUDIO_FILE_PATH);
    mediaPlayList->setPlaybackMode(QMediaPlaylist::Loop);

}

void MainWindow::slotPlayPushButtonClicked()
{
    static bool flag = true;
    if(flag)
        mediaPlayer->play();
    else
        mediaPlayer->pause();
    flag = !flag;

}

void MainWindow::slotnextPushButtonClicked()
{
    mediaPlayList->next();
    mediaPlayer->play();
}

void MainWindow::slotPreviousPushButtonClicked()
{
    mediaPlayList->previous();
    mediaPlayer->play();
}

void MainWindow::mediaPlayerStateChanged(QMediaPlayer::State state)
{
    switch (state) {
    case QMediaPlayer::StoppedState:
        //musicTimer->stop();
        playPushButton->setStyleSheet("QPushButton{border-image:url(Resource/icon/play);outline:none}"
                                      "QPushButton:pressed{border-image:url(Resource/icon/play_p);}");
        break;
    case QMediaPlayer::PlayingState:
        //musicTimer->start(100);
        playPushButton->setStyleSheet("QPushButton{border-image:url(Resource/icon/stop);outline:none}"
                                      "QPushButton:pressed{border-image:url(Resource/icon/stop_p);}");
        break;

    case QMediaPlayer::PausedState:
         //musicTimer->stop();
        playPushButton->setStyleSheet("QPushButton{border-image:url(Resource/icon/play);outline:none}"
                                      "QPushButton:pressed{border-image:url(Resource/icon/play_p);}");
        break;
    }
}
void MainWindow::mediaPlayerMediaChange(QMediaContent)
{
    /* 防止数组越界 */
    int index = mediaPlayList->currentIndex();

    if (-1 == index)
        return;
     songsName->setSongNameText(mediaObjectInfo[mediaPlayList->currentIndex()].fileName);
}

void MainWindow::mediaPlayerPositionChanged(qint64 position)
{
    if(!songsPlaySliderPressFlag)
        musicSlider->setValue(position/1000);

    int second  = position / 1000;
    int minute = second / 60;
    second %= 60;

    QString strLabel;
    strLabel.clear();

    if (minute >= 10)
        strLabel = QString::number(minute, 10);
    else
        strLabel = "0" + QString::number(minute, 10);

    if (second >= 10)
        strLabel = strLabel + ":" + QString::number(second, 10);
    else
        strLabel = strLabel + ":0" + QString::number(second, 10);

    labelDuration->setText(strLabel + strDuration);
}

void MainWindow::mediaPlayerDurationChanged(qint64 duration)
{
    musicSlider->setRange(0,duration/1000);
    int second  = duration / 1000;
    int minute = second / 60;
    second %= 60;

    QString strLabel;
    strLabel.clear();

    if (minute >= 10)
        strLabel = QString::number(minute, 10);
    else
        strLabel = "/0" + QString::number(minute, 10);

    if (second >= 10)
        strLabel = strLabel + ":" + QString::number(second, 10);
    else
        strLabel = strLabel + ":0" + QString::number(second, 10);
    strDuration = strLabel;                                             //媒体总长度

}

void MainWindow::songsPlaySliderPressed()                               //按下时，设置标志位为true，防止歌曲还在播放，滑块还跟着跑
{
    songsPlaySliderPressFlag = true;
}

void MainWindow::songsPlaySliderReleased()                              //当释放进度条滑块时才设置歌曲的播放位置
{
    mediaPlayer->setPosition(musicSlider->value() * 1000);
    songsPlaySliderPressFlag = false;
}

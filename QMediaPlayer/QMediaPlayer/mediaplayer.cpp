#include "mainwindow.h"

//媒体初始化
void MainWindow::mediaPlayerInit()
{

    previousPlaylistIndex = -1;                             //设置默认值为-1
    errorPsition          = 0;                              //出错位置

    mediaPlayer         = new QMediaPlayer(this);
    mediaPlayList       = new QMediaPlaylist(this);
    playerWindow        = new QVideoWidget(mainWidget);

    playerWindow->setGeometry((float)32.5/800*this->geometry().width(), (float)100/480*this->geometry().height(),
                              (float)480/800*this->geometry().width(), (float)272/480*this->geometry().height());//32.5，100，480，272
    mediaPlayList->clear();

    /* 注意，在这里我并不设置mediaPlayer的播放列表为mediaPlayList
     * 因为IMX6使用Qt播放视频能力有限，在处理数据时可能会处理不过来，所以mediaPlayList在播放过程中可能遇到错误。
     * mediaPlayList的Index会自动设置为-1;
     * 至为-1就会重新播放，与实际播放不符合
     * 这样不利于做出错处理的功能。
     */
    //mediaPlayer->setPlaylist(mediaPlayList);              //设置媒体播放器的列表为mediaPlayList
    mediaPlayer->setNotifyInterval(1000);                   //设置触发时间长度

    mediaObjectInfo.clear();                                //清空

    /* 这里不再设置播放列表的模式 */
//    switch (APP_CONFIG_PLAYER_DEFAULT_PLAYBACK_MODE) {
//    case 0:
//        mediaPlayList->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
//        break;

//    default:
//        mediaPlayList->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
//        break;
//    }
}

//双击媒体列表选项播放
void MainWindow::mediaListDoubleCliked(QListWidgetItem* item)
{
    mediaPlayer->stop();
    errorPsition = 0;                                        //出错位置设置值为0
    mediaPlayList->setCurrentIndex(mediaList->row(item));    //setCurrentIndex方法内部会校验index的合理性
    mediaPlayer->setMedia(mediaPlayList->currentMedia());
    mediaPlayer->play();
}

//媒体播放位置改变时，默认媒体会以设置的1000ms触发这个信号，在mediaPlayerInit()里可设置
void MainWindow::mediaPlayerPositionChanged(qint64 position)
{
    if(position != 0)
        errorPsition = position;

    if(!mediaSliderPressFlag)
        mediaSlider->setValue(position/1000);

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

/* 当媒体总长度变化时，设置媒体的总长度标签 */
void MainWindow::mediaPlayerDurationChanged(qint64 duration)
{
    mediaSlider->setRange(0,duration/1000);
    int second  = duration / 1000;
    int minute = second / 60;
    second %= 60;

    QString strLabel;
    strLabel.clear();

    //    if (minute >= 10)
    //        strLabel = QString::number(minute, 10);
    //    else
    strLabel = "/0" + QString::number(minute, 10);

    if (second >= 10)
        strLabel = strLabel + ":" + QString::number(second, 10);
    else
        strLabel = strLabel + ":0" + QString::number(second, 10);
    strDuration = strLabel;                                             //媒体总长度

}

/* 当前媒体改变时 */
void MainWindow::mediaPlayerCurrentMediaChanged(QMediaContent)
{
    int index = mediaPlayList->currentIndex();

    if (-1 == index)
        return;

    if(mediaObjectInfo[mediaPlayList->currentIndex()].fileSuffix != "mp3"
            && mediaObjectInfo[mediaPlayList->currentIndex()].fileSuffix != "wav")//判断是不是视频
    {
        musicLabel->hide();
        playerWindow->show();
    }else{
        playerWindow->hide();
        musicLabel->show();
    }
    mediaPlayer->play();
    labelmediasTitle->setText(mediaObjectInfo[mediaPlayList->currentIndex()].fileName);//注意这里不要放错地方了，否则会越界
}

void MainWindow::mediaPlayerStateChanged(QMediaPlayer::State state)
{
    static bool flag = true;
    if(flag){
        /* 设置媒体的播放窗口为playerWindow，
         * 这里只需要设置一次就可以了，
         * 在其他地方设置按钮可能不可用 */
        mediaPlayer->setVideoOutput(playerWindow);
        flag = false;
    }
    switch (state) {
    case QMediaPlayer::StoppedState: {
        //当停止之后 position为自动变为0 故滑块也会自动清理 所以无需在这里进行处理
        labelmediasTitle->setText("当前无媒体播放!");
        labelDuration->setText("/00:00");

        playOrPausePushButton->setStyleSheet(playOrPausePushButton_STYLE_1);

        //将滑块的数值范围设置为(0, 0)
        mediaSlider->setRange(0, 0);
        //errorPsition = 0;
    }
        break;

    case QMediaPlayer::PlayingState:
        playOrPausePushButton->setStyleSheet(playOrPausePushButton_STYLE_2);
        break;

    case QMediaPlayer::PausedState:
        playOrPausePushButton->setStyleSheet(playOrPausePushButton_STYLE_1);
        break;
    }
}

/* 单击播放/停止按钮 */
void MainWindow::playOrPausePushButtonCliked()
{

    int state = mediaPlayer->state();                                      //获取当前播放器的状态
    switch (state) {                                                       //根据状态做出相应的改变
    case QMediaPlayer::StoppedState:
        if(mediaList->currentRow() == -1 && mediaList->count() !=0 ){      //检查合法性，再播放
            mediaList->setCurrentRow(0);
            mediaPlayList->setCurrentIndex(mediaList->currentRow());
            mediaPlayer->setMedia(mediaPlayList->currentMedia());
            mediaPlayer->play();
        }else{
            mediaPlayList->setCurrentIndex(mediaList->currentRow());
            mediaPlayer->setMedia(mediaPlayList->currentMedia());
            mediaPlayer->play();
        }
        break;

    case QMediaPlayer::PlayingState:
        mediaPlayer->pause();
        break;

    case QMediaPlayer::PausedState:
        mediaPlayer->play();
        break;
    }
}

/* 上一视频按钮点击 */
void MainWindow::previousPushButtonCliked()
{
    mediaPlayer->stop();                                                      //先让播放器处于停止状态

    int count = mediaPlayList->mediaCount();                                  //判断当前播放列表是否为空
    if (0 == count)
        return;

    mediaPlayList->previous();                                                //播放列表切换到上一首
    errorPsition = 0;                                                         //播放出错的位置再设置为0

    /* 校验
     * 一般正常情况下除了单曲播放和单曲循环之外，都是可以切换的
     * 而这两个模式在停止状态下上下切换 index都是 -1
     * 在播放过程中切换 单曲播放index也是 -1
     * 而单曲循环则等于当前正在播放的媒体所对应的index
     * 其实这种设定就是真正的单曲播放模式所对应的这个逻辑关系
     * 不过实际上用户并不接受 所以需要进行修改!
     */
    if (-1 == mediaPlayList->currentIndex()) {

        if (-1 == previousPlaylistIndex)
            mediaPlayList->setCurrentIndex(0);                              //不为空 则至少有一个 设置为0 是合理的
        else {
            if (0 == previousPlaylistIndex)
                mediaPlayList->setCurrentIndex(count - 1);                  //那么它的上一个就是最后一个
            else
                mediaPlayList->setCurrentIndex(previousPlaylistIndex - 1);
        }
    }

    mediaPlayer->setMedia(mediaPlayList->currentMedia());
    mediaPlayer->play();                                                    //执行播放
    labelmediasTitle->setText(mediaObjectInfo[mediaPlayList->currentIndex()].fileName);
}

/* 下一视频按钮点击 */
void MainWindow::nextPushButtonCliked()
{
    mediaPlayer->stop();
    int count = mediaPlayList->mediaCount();
    if (0 == count)
        return;

    mediaPlayList->next();
    errorPsition = 0;                                                         //播放出错的位置再设置为0


    if (-1 == mediaPlayList->currentIndex()) {

        if (-1 == previousPlaylistIndex)
            mediaPlayList->setCurrentIndex(0);
        else {
            if (count - 1 == previousPlaylistIndex)
                mediaPlayList->setCurrentIndex(0);
            else
                mediaPlayList->setCurrentIndex(previousPlaylistIndex + 1);
        }
    }

    mediaPlayer->setMedia(mediaPlayList->currentMedia());
    mediaPlayer->play();
    labelmediasTitle->setText(mediaObjectInfo[mediaPlayList->currentIndex()].fileName);
}

/* 注意:媒体有可能遇到错误 */
void MainWindow::mediaPlayerError(QMediaPlayer::Error error)
{
    /* 有些性能较低的机子，当双击全屏容易出现视频流错误，
     * 也就是ResourceError
     * 这里只做了从播放出错的位置重新播放*/
    if(error == QMediaPlayer::ResourceError){
        mediaPlayList->setCurrentIndex(mediaList->currentRow());
        mediaPlayer->setMedia(mediaPlayList->currentMedia());
        mediaPlayer->setPosition(errorPsition);
        mediaPlayer->play();
        labelmediasTitle->setText(mediaObjectInfo[mediaPlayList->currentIndex()].fileName);
    }else{
        previousPlaylistIndex = -1;
    }


}

void MainWindow::mediaPlayerStatusChanged(QMediaPlayer::MediaStatus status)
{
    if(status == QMediaPlayer::EndOfMedia)
      nextPushButtonCliked();                           //当媒体结束时，这里自动切下一个
}

void MainWindow::mutePushButtonCliked()
{
    static int preVolume = volumeslider->value();       // 一开始读取滑块的数值 应该是等于默认值

    if (0 == volumeslider->value())
        volumeslider->setValue(preVolume);              // 滑块数值一变就会触发绑定的槽函数 会直接设置播放器的音量
    else {
        preVolume = volumeslider->value();              // 先保存滑块此时的数值
        volumeslider->setValue(0);                      // 置为静音状态
    }
}

//当媒体播放列表Index改变时
void MainWindow::mediaPlayListCurrentIndexChanged(int index)
{
    if (-1 == index)                                                //判断当前改变的index是否合理
        return;

    previousPlaylistIndex = index;                                   //previousPlaylistIndex
    labelmediasTitle->setText(mediaObjectInfo[index].fileName);      //显示当前播放的媒体名称
    mediaList->setCurrentRow(index);                                 //设置当前列表的焦点
}

void MainWindow::mediaSliderPressed()                                //按下时，设置标志位为true，防止媒体还在播放，滑块还跟着跑
{
    mediaSliderPressFlag = true;
}

void MainWindow::mediaSliderReleased()                               //当释放进度条滑块时才设置媒体的播放位置
{
    mediaPlayer->setPosition(mediaSlider->value() * 1000);
    mediaSliderPressFlag = false;
}

//当前音量值改变时
void MainWindow::volumeSliderValueChanged(int volume)
{
    mediaPlayer->setVolume(volume);
    if(mediaPlayer->volume() == 0)
    {
        mutePushButton->setStyleSheet(MUTEPUSHBUTTON_STYLE_2);
    }else{
        mutePushButton->setStyleSheet(MUTEPUSHBUTTON_STYLE_1);
    }
}




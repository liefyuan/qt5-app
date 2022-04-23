#include "mainwindow.h"

void MainWindow::mediaplayerInit()
{
    previousPlaylistIndex   = -1;
    mediaSliderPressFlag    = false;
    mediaplayer             = new QMediaPlayer(this);
    mediaplaylist           = new QMediaPlaylist(this);

    /* 设置媒体的播放列表为mediaplaylist */
    mediaplayer->setPlaylist(mediaplaylist);
    mediaplaylist->setPlaybackMode(QMediaPlaylist::Loop);
    mediaplaylist->clear();
    mediaplayer->setVolume(USER_VOLUME);
    mediaObjectInfo.clear();                                //清空
}

void MainWindow::scanAudio()
{
    /* 扫描/添加本地磁盘音频文件，默认为扫描目录Resource/audio/ */
    QDir dir1(APP_CONFIG_AUDIO_FILE_PATH);
    /* 绝对路径转换，如果不转换则linux底层的播放器会找不到文件 */
    QDir dir2(dir1.absolutePath());
    mediaObjectInfo.clear();
    mediaplaylist->clear();
    recorderListWidget->clear();
    if (dir2.exists()) {
        /* 定义过滤器 */
        QStringList filter;
        filter << "*.mp3"<< "*.wav"<< "*.mp4"<< "*.avi"<< "*.mkv";
        QFileInfoList files = dir2.entryInfoList(filter, QDir::Files);
        for (int i = 0; i < files.count(); i++) {
            /* 使用Utf8编码 */
            MediaObjectInfo info;
            info.fileName = QString::fromUtf8(files.at(i).fileName().toUtf8().data());
            info.filePath = QString::fromUtf8(files.at(i).filePath().toUtf8().data());
            recorderListWidget->addItem(QString::fromUtf8(files.at(i).fileName().toUtf8().data()));
            if(mediaplaylist->addMedia(QUrl::fromLocalFile(QString::fromUtf8(files.at(i).filePath().toUtf8().data()))))
                mediaObjectInfo.append(info);
        }
        QListWidgetItem *item = NULL;
        QFont font;
        font.setPixelSize((float)15/600*mainWidget->geometry().width());
        /* 设置项样式 */
        for (int i = 0; i <recorderListWidget->count(); i++) {
            item = recorderListWidget->item(i);
            item->setForeground(Qt::white);
            item->setSizeHint(QSize((float)200/600*mainWidget->geometry().width(), (float)25/250*mainWidget->geometry().height()));
            item->setFont(font);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            item->setToolTip(files.at(i).filePath());
            //item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);// 设置可以编辑
        }
    }
}

/* 双击录音列表选项播放 */
void MainWindow::recorderListWidgetDoubleCliked(QListWidgetItem* item)
{
    mediaplayer->stop();
    mediaplaylist->setCurrentIndex(recorderListWidget->row(item));    //setCurrentIndex方法内部会校验index的合理性
    mediaplayer->play();
}

void MainWindow::recorderListWidgetCurrentRowChanged(int currtenRow)
{
    if(currtenRow == -1) // 检查是否有选中录音列表里的内容
        return;
    deletePushButton->show();
}

/* 媒体播放位置改变时，滑条的位置也会改变 */
void MainWindow::mediaplayerPositionChanged(qint64 position)
{
    int second  = position / 1000;
    int minute = second / 60;
    second %= 60;

    QString strLabel;
    strLabel.clear();
    mediaSlider->setValue(position/1000);

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
void MainWindow::mediaplayerDurationChanged(qint64 duration)
{
    if(!mediaSliderPressFlag)
        mediaSlider->setRange(0,duration/1000);
    int second  = duration / 1000;
    int minute = second / 60;
    second %= 60;

    QString strLabel;
    strLabel.clear();

    strLabel = "/0" + QString::number(minute, 10);

    if (second >= 10)
        strLabel = strLabel + ":" + QString::number(second, 10);
    else
        strLabel = strLabel + ":0" + QString::number(second, 10);
    strDuration = strLabel;                                             //媒体总长度
}

/* 单击播放/停止按钮 */
void MainWindow::playOrPausePushButtonCliked()
{
    int state = mediaplayer->state();                                   //获取当前播放器的状态
    switch (state) {                                                    //根据状态做出相应的改变
    case QMediaPlayer::StoppedState:
        mediaplayer->play();
        break;

    case QMediaPlayer::PlayingState:
        mediaplayer->pause();
        break;

    case QMediaPlayer::PausedState:
        mediaplayer->play();
        break;
    }
}

void MainWindow::mediaplayerStateChanged(QMediaPlayer::State state)
{
    switch (state) {
    case QMediaPlayer::StoppedState: {
        labelDuration->setText("/00:00");
        playOrPausePushButton->setStyleSheet(PLAYORPAUSEPUSHBUTTON_STYLE_1);
        /* 将滑块的数值范围设置为(0, 0) */
        mediaSlider->setRange(0, 0);
    }
        break;

    case QMediaPlayer::PlayingState:
        playOrPausePushButton->setStyleSheet(PLAYORPAUSEPUSHBUTTON_STYLE_2);
        break;

    case QMediaPlayer::PausedState:
        playOrPausePushButton->setStyleSheet(PLAYORPAUSEPUSHBUTTON_STYLE_1);
        break;
    }
}

/* 当媒体播放列表Index改变时 */
void MainWindow::mediaplaylistCurrentIndexChanged(int index)
{
    if (-1 == index)                                                // 判断当前改变的index是否合理
        return;

    recorderListWidget->setCurrentRow(index);                       // 设置当前列表的焦点
    previousPlaylistIndex = index;                                  // previousPlaylistIndex
    labelmediaTitle->setText(mediaObjectInfo[index].fileName);      // 显示当前播放的媒体名称
}

/* 下一首 */
void MainWindow::nextPushButtonCliked()
{
    mediaplayer->stop();
    int count = mediaplaylist->mediaCount();
    if (0 == count)
        return;

    mediaplaylist->next();


    if (-1 == mediaplaylist->currentIndex()) {

        if (-1 == previousPlaylistIndex)
            mediaplaylist->setCurrentIndex(0);
        else {
            if (count - 1 == previousPlaylistIndex)
                mediaplaylist->setCurrentIndex(0);
            else
                mediaplaylist->setCurrentIndex(previousPlaylistIndex + 1);
        }
    }

    mediaplayer->play();
}

/* 上一首 */
void MainWindow::previousPushButtonCliked()
{
    mediaplayer->stop();                                                      //先让播放器处于停止状态

    int count = mediaplaylist->mediaCount();                                  //判断当前播放列表是否为空
    if (0 == count)
        return;

    mediaplaylist->previous();                                                //播放列表切换到上一首

    /* 校验
     * 一般正常情况下除了单曲播放和单曲循环之外，都是可以切换的
     * 而这两个模式在停止状态下上下切换 index都是 -1
     * 在播放过程中切换 单曲播放index也是 -1
     * 而单曲循环则等于当前正在播放的歌曲所对应的index
     * 其实这种设定就是真正的单曲播放模式所对应的这个逻辑关系
     * 不过实际上用户并不接受 所以需要进行修改!
     */
    if (-1 == mediaplaylist->currentIndex()) {

        if (-1 == previousPlaylistIndex)
            mediaplaylist->setCurrentIndex(0);                              //不为空 则至少有一个 设置为0 是合理的
        else {
            if (0 == previousPlaylistIndex)
                mediaplaylist->setCurrentIndex(count - 1);                  //那么它的上一个就是最后一个
            else
                mediaplaylist->setCurrentIndex(previousPlaylistIndex - 1);
        }
    }

    mediaplayer->play();                                                    //执行播放
}

void MainWindow::mediaSliderPressed()
{
    mediaSliderPressFlag = true;         // 按下标志位为true
}

void MainWindow::mediaSliderReleased()
{
    mediaplayer->setPosition(mediaSlider->value() * 1000);
    mediaSliderPressFlag = false;       // 按下标志位为false
}

void MainWindow::volumeSliderValueChanged(int volume)
{
    mediaplayer->setVolume(volume);

    /* 当音量值为0时，设置按钮为静音按钮的样式 */
    if(mediaplayer->volume() == 0)
    {
        mutePushButton->setStyleSheet(MUTEPUSHBUTTON_STYLE_2);
    }else{
        mutePushButton->setStyleSheet(MUTEPUSHBUTTON_STYLE_1);
    }
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

void MainWindow::recorderButtonCliked()
{
    static bool startFlag = true;                       // 定义否是始录音
    static  QString currentDate;
    if(startFlag){
        currentDate = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        recorderButton->setStyleSheet(RECORDERBUTTON_STYLE_2);

        /* 开始录音时，先把扬声器和耳机关掉，否则扬声器会对mic影响，也就是把输出禁用 */
        processCmd->start("amixer sset 'Left Output Mixer PCM' off");
        processCmd->waitForFinished(-1);
        processCmd->start("amixer sset 'Right Output Mixer PCM' off");
        processCmd->waitForFinished(-1);

        QDir dir("./Resource/audio");                   // 判断是否存在这样的目录，如果不存在就创建这样的目录
        if(dir.exists())
            dir.mkpath("../../Resource/audio");

        /* 执行这条指令会开始录音，直到按结束键为止 */
        processCmd->start("arecord -r 44100 -f S16_LE -c 2 ./Resource/audio/"+ currentDate +".wav");
        timerRecorder->start(1000);
        labelRecorder->show();

    }else{
        recorderButton->setStyleSheet(RECORDERBUTTON_STYLE_1);
        /* 结束录音 */
        processCmd->close();                            // 关闭进程

        /* 录音完成后，开启扬声器、耳机 */
        processCmd->start("amixer sset 'Left Output Mixer PCM' on");
        processCmd->waitForFinished(-1);
        processCmd->start("amixer sset 'Right Output Mixer PCM' on");
        processCmd->waitForFinished(-1);

        /* 停止定时器，并将录音时长设置为0 */
        timerRecorder->stop();

        /* 窗口讯问，是否要保存录音文件 */
        QMessageBox *msgBox = new QMessageBox(this);
        msgBox->setStyleSheet("QWidget{border-image: transparent}");
        msgBox->setText("是否要保存"+ currentDate + ".wav?");

        QPushButton *btn_yes = msgBox->addButton("确定", QMessageBox::AcceptRole);
        QPushButton *btn_cancel = msgBox->addButton("取消", QMessageBox::RejectRole);
        btn_yes->setFocusPolicy(Qt::NoFocus);
        btn_cancel->setFocusPolicy(Qt::NoFocus);
        btn_yes->setStyleSheet(BTN_YES_STYLE);
        btn_cancel->setStyleSheet(BTN_CANCEL_STYLE);

        /* 如果用户选择了确认，则把录音文件添加至列表中 */
        if(msgBox->exec()==QMessageBox::AcceptRole)
            addAudiofile(currentDate);

        recorderDuration = 0;
        labelRecorder->hide();
    }
    startFlag = !startFlag;                             // 取反
}

/* 记录录音时长 */
void MainWindow::timerRecorderTimeOut()
{
    recorderDuration++;
    int second  = recorderDuration;
    int minute = second / 60;
    second %= 60;

    QString strLabel;
    strLabel.clear();

    strLabel = "0" + QString::number(minute, 10);

    if (second >= 10)
        strLabel = strLabel + ":" + QString::number(second, 10);
    else
        strLabel = strLabel + ":0" + QString::number(second, 10);

    labelRecorder->setText(strLabel);
}

void MainWindow::addAudiofile(QString fileName)
{
    QString strFilePath = "./Resource/audio/";
    QDir dir1(strFilePath);

    /* 绝对路径转换 */
    QDir dir2(dir1.absolutePath());
    QString recorderFile = dir2.path() + "/" + fileName + ".wav";

    QFile file(recorderFile);
    if(file.exists()){
        QFileInfo fileInfo(file);
        MediaObjectInfo info;
        info.fileName = QString::fromUtf8(fileInfo.fileName().toUtf8().data()); // 使用utf-8编码
        info.filePath = QString::fromUtf8(fileInfo.filePath().toUtf8().data());
        if (mediaplaylist->addMedia(QUrl::fromLocalFile(info.filePath)))
            mediaObjectInfo.append(info);
        QListWidgetItem *item = NULL;
        recorderListWidget->addItem(mediaObjectInfo[recorderListWidget->count()].fileName);
        QFont font;
        font.setPixelSize((float)15/600*mainWidget->geometry().width());

        item = recorderListWidget->item(recorderListWidget->count()-1);
        item->setForeground(Qt::white);
        item->setSizeHint(QSize((float)200/600*mainWidget->geometry().width(), (float)25/250*mainWidget->geometry().height()));
        item->setFont(font);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item->setToolTip(mediaObjectInfo[recorderListWidget->count()-1].filePath);
        //item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);// 设置可以编辑
        recorderListWidget->setCurrentRow(recorderListWidget->count()-1);
    }
}

void MainWindow::mediaplayerCurrentMediaChanged(QMediaContent)
{
    int index = mediaplaylist->currentIndex();

    if (-1 == index)                                    // 检查合法性
        return;

    /* 设置当前播放音频的名称 */
    labelmediaTitle->setText(mediaObjectInfo[mediaplaylist->currentIndex()].fileName);
}

void MainWindow::deleteFile()
{
    if(recorderListWidget->currentRow() == -1)          // 检验合法性
        return;

    /* 当前要删除的文件 */
    QFile file(mediaObjectInfo[recorderListWidget->currentRow()].filePath);

    if(!file.exists())      //检验是否存在这样的文件
        return;

    /* 窗口对话框 */
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setStyleSheet("QWidget{border-image: transparent}");
    msgBox->setText("确认要删除" + mediaObjectInfo[recorderListWidget->currentRow()].fileName + "?\n"
                    "执行删除会重新载入列表，确认当前没有音频文件正在播放!"
            );

    QPushButton *btn_yes = msgBox->addButton("确定", QMessageBox::AcceptRole);
    QPushButton *btn_cancel = msgBox->addButton("取消", QMessageBox::RejectRole);
    btn_yes->setFocusPolicy(Qt::NoFocus);
    btn_cancel->setFocusPolicy(Qt::NoFocus);
    btn_yes->setStyleSheet(BTN_YES_STYLE);
    btn_cancel->setStyleSheet(BTN_CANCEL_STYLE);

    /* 判断是否要删除文件 */
    if(msgBox->exec()==QMessageBox::AcceptRole){
        // TODO
    }else{
        return;         //在此返回，后面的不会操作了
    }

    /* 移除项 */
    recorderListWidget->takeItem(recorderListWidget->currentRow());

    /* 删除文件 */
    file.remove();

    /* 删除文件后，重新载入列表 */
    scanAudio();

    /* 删除完，没有文件可以删除了就设置按钮为不可见状态 */
    if(recorderListWidget->count() == 0)
        deletePushButton->hide();
    else
        recorderListWidget->setCurrentRow(0);

    /* 设置标题 */
    labelmediaTitle->setText("此刻，记录周围美好的声音!");

}

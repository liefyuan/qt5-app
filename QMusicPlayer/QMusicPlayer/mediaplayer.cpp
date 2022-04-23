#include "mainwindow.h"

void MainWindow::mediaPlayerInit()
{

    previousPlaylistIndex = -1;                             //设置默认值为-1

    mediaPlayer           = new QMediaPlayer(this);
    mediaPlayList         = new QMediaPlaylist(this);
    mediaPlayList->clear();

    mediaPlayer->setPlaylist(mediaPlayList);                //设置媒体播放器的列表为mediaPlayList
    mediaPlayer->setNotifyInterval(1000);                   //设置触发时间长度

    mediaObjectInfo.clear();                                //清空
}

//双击歌曲列表选项播放
void MainWindow::songListCliked(QListWidgetItem* item)
{
    mediaPlayer->stop();
    mediaPlayList->setCurrentIndex(songList->row(item));    //setCurrentIndex方法内部会校验index的合理性
    mediaPlayer->play();
}

void MainWindow::playbackModeListItemEnter(QListWidgetItem *item)
{
    static QListWidgetItem *preItem = NULL;                                 // 用来记录上一个item
    QListWidgetItem *currentSectionItem = playBackModeList->currentItem();  // 表示当前鼠标单击选中的item

    if (preItem) {
        if (preItem != currentSectionItem &&
                preItem != item) {
            preItem->setForeground(QColor(210, 210, 210));
            preItem->setIcon(QIcon(pbModeArr[playBackModeList->row(preItem)].icon1));
        }
    }

    if (item == currentSectionItem ||
            item == preItem) {
        preItem = item;
        return;
    }

    item->setForeground(QColor(255, 255, 255));                             // 显示加亮
    item->setIcon(QIcon(pbModeArr[playBackModeList->row(item)].icon2));     // 图标加亮
    preItem = item;
}

void MainWindow::playbackModeListItemCliked(QListWidgetItem *)
{
    playBackModeList->setVisible(false);                                    //设置播放模式列表不可见
}

void MainWindow::playbackModeListCurrentItemChange(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (previous) {  // 为什么需要在这里判断 主要是因为如果是第一次点  那么这个值肯定是空值
        previous->setForeground(QColor(210, 210, 210));
        previous->setIcon(QIcon(pbModeArr[playBackModeList->row(previous)].icon1));
    }

    current->setForeground(QColor(255, 255, 255));
    current->setIcon(QIcon(pbModeArr[playBackModeList->row(current)].icon2));

    //更改播放模式
    int index = playBackModeList->row(current);
    switch (index) {
    case 0:
        mediaPlayList->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        playbackMode->setStyleSheet(PLAYBACKMODE_STYLE_0);
        break;

    case 1:
        mediaPlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        playbackMode->setStyleSheet(PLAYBACKMODE_STYLE_1);
        break;

    case 2:
        mediaPlayList->setPlaybackMode(QMediaPlaylist::Sequential);
        playbackMode->setStyleSheet(PLAYBACKMODE_STYLE_2);
        break;

    case 3:
        mediaPlayList->setPlaybackMode(QMediaPlaylist::Loop);
        playbackMode->setStyleSheet(PLAYBACKMODE_STYLE_3);
        break;

    case 4:
        mediaPlayList->setPlaybackMode(QMediaPlaylist::Random);
        playbackMode->setStyleSheet(PLAYBACKMODE_STYLE_4);
        break;
    }

}

void MainWindow::settingsListItemCliked(QListWidgetItem *item)
{
    switch (settingsList->row(item)) {
    case 0:
        if(item->text() == "关闭写真"){
            item->setText("开启写真");
            this->setStyleSheet(APP_CONFIG_MAINWINDOW_BACKGROUND_IMAGE);    //否则默认使用桌面背景
        }else{
            item->setText("关闭写真");
            if (mediaPlayList->currentIndex() == -1)                        //判断当前播放列表的下标
                return;
            if(!(findPhotoFunc() == "yes"))                                 //如果没有找到写真图片
                this->setStyleSheet(APP_CONFIG_MAINWINDOW_BACKGROUND_IMAGE);//否则默认使用桌面背景
        }
        break;
    case 1://预留

        break;
    default:
        break;
    }
}

//媒体播放位置改变时，默认媒体会以设置的1000ms触发这个信号，在mediaPlayerInit()里可设置
void MainWindow::mediaPlayerPositionChanged(qint64 position)
{
    if(!songsPlaySliderPressFlag)
        songsPlaySlider->setValue(position/1000);

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

    //歌词显示更新
    int count = lrcArr.count();
    for (int i = 0; i < count; i++) {

        if (lrcArr[i].time == strLabel) {

            labelArr[3]->setText(lrcArr[i].content);
            if (i-1 >= 0)
                labelArr[2]->setText(lrcArr[i-1].content);
            else
                labelArr[2]->clear();

            if (i-2 >= 0)
                labelArr[1]->setText(lrcArr[i-2].content);
            else
                labelArr[1]->clear();

            if (i-3 >= 0)
                labelArr[0]->setText(lrcArr[i-3].content);
            else
                labelArr[0]->clear();

            if (i+1 < count)
                labelArr[4]->setText(lrcArr[i+1].content);
            else
                labelArr[4]->clear();

            if (i+2 < count)
                labelArr[5]->setText(lrcArr[i+2].content);
            else
                labelArr[5]->clear();

            if (i+3 < count)
                labelArr[6]->setText(lrcArr[i+3].content);
            else
                labelArr[6]->clear();
        }
    }
}

void MainWindow::mediaPlayerDurationChanged(qint64 duration)
{
    songsPlaySlider->setRange(0,duration/1000);
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

void MainWindow::mediaPlayerCurrentMediaChanged(QMediaContent)
{
    int index = mediaPlayList->currentIndex();

    if (-1 == index)
        return;

    labelSongsTitle->setText(mediaObjectInfo[mediaPlayList->currentIndex()].fileName);//注意这里不要放错地方了

    lrcArr.clear();                                                     // 释放上一首歌曲的歌词所占内存
    for (int i = 0; i < 7; i++){                                        // 清理歌词显示label
        labelArr[i]->clear();
    }

    //mediaPlayer->pause();                                             //先停止播放，等待图片加载完成再播放音乐
    if(!(findPhotoFunc() == "yes"))                                     //如果没有找到写真图片
        this->setStyleSheet(APP_CONFIG_MAINWINDOW_BACKGROUND_IMAGE);    //否则默认使用桌面背景
    //mediaPlayer->play();

    QString lrcFile =  "/" + mediaObjectInfo[index].fileName + ".lrc";  //获取文件名以及路径
    lrcFile = APP_CONFIG_LRC_FILE_PATH + lrcFile;
    QFile file(lrcFile);

    if (!file.exists())                                                 //判断文件是否存在
        return;                                                         //不存在则返回

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))              //尝试打开文件
        return;

    QTextCodec *code = QTextCodec::codecForName("UTF-8");               //设置读取文件的编码方式，这里注意了，歌词文本要是”UTF-8“的，否则会乱码
    QTextStream in(&file);
    in.setCodec(code);

    in.readLine();
    in.readLine();                                                      //读取文件，跳过前面两行，所以这里是读取两次
    QString line;
    LrcFormat info;

    while (!in.atEnd()) {
        line = in.readLine();
        info.time = line.mid(1,5);
        info.content = line.mid(10);
        lrcArr.append(info);
    }
    file.close();                                                        //读取完成之后 别忘了关闭文件
    int count = lrcArr.count();                                          //显示最前面的几句歌词
    if (count > 0)
        labelArr[4]->setText(lrcArr[0].content);
    if (count > 1)
        labelArr[5]->setText(lrcArr[1].content);
    if (count > 2)
        labelArr[6]->setText(lrcArr[2].content);

}

void MainWindow::mediaPlayerStateChanged(QMediaPlayer::State state)
{
    switch (state) {
    case QMediaPlayer::StoppedState: {
        //当停止之后 position为自动变为0 故滑块也会自动清理 所以无需在这里进行处理
        labelSongsTitle->setText("Q音乐 极致享受!");
        labelDuration->setText("/00:00");

        playOrPausePushButton->setStyleSheet(playOrPausePushButton_STYLE_1);
        for (int i = 0; i < 7; i++){
            labelArr[i]->clear();
        }
        this->setStyleSheet(APP_CONFIG_MAINWINDOW_BACKGROUND_IMAGE);//使用默认背景
        //将滑块的数值范围设置为(0, 0)
        songsPlaySlider->setRange(0, 0);
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

//单击播放/停止按钮
void MainWindow::playOrPausePushButtonCliked()
{
    int state = mediaPlayer->state();                                      //获取当前播放器的状态

    switch (state) {                                                       //根据状态做出相应的改变
    case QMediaPlayer::StoppedState:
        mediaPlayer->play();
        break;

    case QMediaPlayer::PlayingState:
        mediaPlayer->pause();
        break;

    case QMediaPlayer::PausedState:
        mediaPlayer->play();
        break;
    }
}

//上一曲按钮点击
void MainWindow::previousPushButtonCliked()
{
    mediaPlayer->stop();                                                      //先让播放器处于停止状态

    int count = mediaPlayList->mediaCount();                                  //判断当前播放列表是否为空
    if (0 == count)
        return;

    mediaPlayList->previous();                                                //播放列表切换到上一首

    /* 校验
     * 一般正常情况下除了单曲播放和单曲循环之外，都是可以切换的
     * 而这两个模式在停止状态下上下切换 index都是 -1
     * 在播放过程中切换 单曲播放index也是 -1
     * 而单曲循环则等于当前正在播放的歌曲所对应的index
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

    mediaPlayer->play();                                                    //执行播放
}

//下一曲按钮点击
void MainWindow::nextPushButtonCliked()
{
    mediaPlayer->stop();
    int count = mediaPlayList->mediaCount();
    if (0 == count)
        return;

    mediaPlayList->next();


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

    mediaPlayer->play();
}

//注意:媒体有可能遇到错误
void MainWindow::mediaPlayerError(QMediaPlayer::Error)
{

    previousPlaylistIndex = -1;
    //可在这里填写你的错误处理方法
}

void MainWindow::mediaPlayerStatusChanged(QMediaPlayer::MediaStatus)
{
    //TODO   
}

QString MainWindow::findPhotoFunc()
{
    QString a = "yes";
    int index = mediaPlayList->currentIndex();
    if(settingsList->item(0)->text() == "关闭写真"){
        QDir dir(APP_CONFIG_SINGER_PHOTO_FILE_PATH);
        if (dir.exists()) {
            QStringList filter;
            filter << "*.png";
            QFileInfoList files = dir.entryInfoList(filter, QDir::Files);   // 扫描该目录下的所有匹配到的文件
            for (int i = 0; i < files.count(); i++) {                       // 遍历所有文件
                if (files.at(i).fileName().replace(".png", "")
                        == mediaObjectInfo[index].fileName) {
                    QString qstrStyleSheet;
                    qstrStyleSheet.clear();
                    qstrStyleSheet = "QMainWindow{border-image:url";
                    qstrStyleSheet = qstrStyleSheet + "(" + files.at(i).filePath() + ")}";
                    this->setStyleSheet(qstrStyleSheet);
                    //mediaPlayer->play();
                    return a;                                               //如果有这样的写真图片，就返回
                }
            }
        }
    }
    a = "no";
    return a;
}
//当播放模式按钮按下时
void MainWindow::playbackModeCliked()
{
    if (playBackModeList->isVisible()) {                            //检测是否可见，状态取反
        playBackModeList->setVisible(false);
    }else {
        playBackModeList->setVisible(true);
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

//当媒体播放列表Index改变时
void MainWindow::mediaPlayListCurrentIndexChanged(int index)
{

    if (-1 == index)                                                //判断当前改变的index是否合理
        return;

    previousPlaylistIndex = index;                                  //previousPlaylistIndex
    labelSongsTitle->setText(mediaObjectInfo[index].fileName);      //显示当前播放的媒体名称
    songList->setCurrentRow(index);                                 //设置当前列表的焦点
}

void MainWindow::songsPlaySliderPressed()                           //按下时，设置标志位为true，防止歌曲还在播放，滑块还跟着跑
{
    songsPlaySliderPressFlag = true;
}

void MainWindow::songsPlaySliderReleased()                          //当释放进度条滑块时才设置歌曲的播放位置
{
    mediaPlayer->setPosition(songsPlaySlider->value() * 1000);
    songsPlaySliderPressFlag = false;
}

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




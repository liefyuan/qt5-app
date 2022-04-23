#include <mainwindow.h>
#include <QEventLoop>

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == playBackModeList){                         //定义事件过滤器，指定在播放模式列表上检测
        switch (event->type()) {
        case QEvent::Leave:                                  //离开时设置当前选择的项为高亮，其他设置为原来的
            playBackModeList->currentItem()->setForeground(QColor(255, 255, 255));
            playBackModeList->currentItem()->setIcon(QIcon(pbModeArr[playBackModeList->currentRow()].icon2));;
            for (int i = 0; i < 5; i++) {
                if(i != playBackModeList->currentRow() )
                    playBackModeList->item(i)->setIcon(QIcon(pbModeArr[i].icon1));
            }
            break;
        default:
            break;
        }
    }

    if(watched == mutePushButton){                            //定义事件过滤器，指定在静音按钮上检测
        switch (event->type()) {
        case QEvent::Enter:
            volumeslider->setVisible(true);
            timerVolumeSlider->stop();
            break;

        case QEvent::MouseButtonPress:
            break;

        case QEvent::Leave:
            timerVolumeSlider->start(1000);
            break;
        default:
            break;
        }
    }

    if(watched == volumeslider){                            //定义事件过滤器，指定在音量条上检测
        switch (event->type()) {
        case QEvent::Leave:
            volumeslider->setVisible(false);                //设置音量条不可见
            break;

        case QEvent::Enter:
            timerVolumeSlider->stop();                      //停止音量条定时器
            break;

        case QEvent::MouseButtonPress:
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            float posPosition;
            posPosition = (float)mouseEvent->pos().y()/volumeslider->geometry().height();
            volumeslider->setValue((1 - posPosition)*volumeslider->maximum());
            mediaPlayer->setVolume(volumeslider->value());
        }
            break;

        default:
            break;
        }
    }

    if(watched == songsPlaySlider){                         //定义事件过滤器，指定在音量条上检测
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            songsPlaySliderPressFlag = true;
            if(songsPlaySliderPressFlag){
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                float posPosition;
                posPosition = (float)mouseEvent->pos().x()/songsPlaySlider->geometry().width();
                songsPlaySlider->setValue(posPosition*songsPlaySlider->maximum());
                mediaPlayer->setPosition(songsPlaySlider->value() * 1000);
            }
            break;

        case QEvent::MouseButtonRelease:
            songsPlaySliderPressFlag = false;
            break;

        default:
            break;
        }
    }

    if(watched == settingsList){                            //定义事件过滤器，指定在设置列表上检测
        switch (event->type()) {
        case QEvent::Leave:
            settingsList->setCurrentRow(-1);
            break;

        default:
            break;
        }
    }

    return QWidget::eventFilter(watched,event);//将事件传递给父类
}

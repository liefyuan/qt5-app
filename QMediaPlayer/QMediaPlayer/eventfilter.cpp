#include <mainwindow.h>
#include <QEventLoop>
//事件过滤器单独一个cpp文件，因为太长
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    static QPoint point;
    static bool   doubleClikedFlag = false;
    if(watched == this){                                                //定义事件过滤器，指定在主窗口上检测
        switch (event->type()) {
        case QEvent::MouseButtonPress:
        {
            point.setY(cursor().pos().y());                             //记录按下点的y坐标

        }
            toolWidgetPanel->show();
            if(!mediaList->isVisible()) {
                timerPanelShow->start(8000);                            //用列表是否可见的状态来开启定时器
            }

            break;
        case QEvent::MouseButtonDblClick:
            if(!doubleClikedFlag){
                //全屏
                playerWindow->setGeometry(0, 0, this->geometry().width(), this->geometry().height());
                mediaList->hide();
                mainWidget->setStyleSheet(MAINWIDGET_STYLE_2);
                timerPanelShow->start(2000);
                musicLabel->setGeometry((float)300/800*this->geometry().width(),
                                        (float)200/480*this->geometry().height(),
                                        (float)200/800*this->geometry().width(),
                                        (float)25/480*this->geometry().height());//350,200,350,25
                enterexitanimation->hide();

            }else{
                //非全屏
                playerWindow->setGeometry((float)32.5/800*this->geometry().width(), (float)100/480*this->geometry().height(),
                                          (float)480/800*this->geometry().width(), (float)272/480*this->geometry().height());//280，100，480，272
                mediaList->show();
                mainWidget->setStyleSheet(MAINWIDGET_STYLE_1);
                musicLabel->setGeometry((float)200/800*this->geometry().width(),
                                        (float)200/480*this->geometry().height(),
                                        (float)200/800*this->geometry().width(),
                                        (float)25/480*this->geometry().height());//350,200,350,25
                timerPanelShow->stop();
                enterexitanimation->show();
            }
            doubleClikedFlag = !doubleClikedFlag;
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

    if(watched == mediaSlider){                             //定义事件过滤器，指定在音量条上检测
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            mediaSliderPressFlag = true;
            if(mediaSliderPressFlag){
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                float posPosition;
                posPosition = (float)mouseEvent->pos().x()/mediaSlider->geometry().width();
                mediaSlider->setValue(posPosition*mediaSlider->maximum());
                mediaPlayer->setPosition(mediaSlider->value() * 1000);
            }
            break;

        case QEvent::MouseButtonRelease:
            mediaSliderPressFlag = false;
            break;

        default:
            break;
        }
    }

    return QWidget::eventFilter(watched,event);             //将事件传递给父类
}

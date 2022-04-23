#include "vscroll.h"

Vscroll::Vscroll(QWidget *parent) : QWidget(parent)
{
    setMaximumSize(0,0);
}

void Vscroll::setObjectScrollBar(QScrollBar *obj)
{
  m_scrollBarV =  obj;
}

void Vscroll::setObjectList(QWidget *obj)
{
    QObjectList objectList = obj->children();
    for(int i = 0; i < objectList.count(); i++)
    {
        if(objectList.at(i)->objectName() == "qt_scrollarea_viewport")
        {
            objectList.at(i)->installEventFilter(this);
        }
    }
}

bool Vscroll::eventFilter(QObject *obj, QEvent *event)
{
    static int press_y   = 0;
    static int move_y    = -1;
    static int release_y = 0;
    static QDateTime pressDateTime;
    static QPropertyAnimation *animation = new QPropertyAnimation();

    if("qt_scrollarea_viewport" != obj->objectName())
    {
        return false;
    }

    int scrollV_max = m_scrollBarV->maximum ();
    int scrollV_min = m_scrollBarV->minimum ();

    //根据鼠标的动作——按下、放开、拖动，执行相应的操作
    if(event->type() == QEvent::MouseButtonPress)
    {
        //记录按下的时间、坐标

        pressDateTime = QDateTime::currentDateTime();
        move_y  = QCursor::pos().y();
        press_y = move_y;

        animation->stop();
    }
    else if(event->type() == QEvent::MouseButtonRelease)
    {
        //鼠标放开，根据鼠标拖动的垂直距离和持续时间，设置窗口滚动快慢程度和距离

        if(animation->targetObject() != m_scrollBarV)
        {
            animation->setTargetObject(m_scrollBarV);
            animation->setPropertyName("value");
        }

        move_y = -1;
        release_y = QCursor::pos().y();
//        qDebug()<<"MouseButtonRelease QCursor::pos().y()="<<QCursor::pos().y();
        QObject *parent_obj = obj->parent();
        if(parent_obj != 0 || parent_obj->inherits("QAbstractItemView"))
        {
//            QTimer::singleShot(150, (QAbstractItemView *)parent_obj
//                               , SLOT(clearSelection()));
        }

        int endValue;
        int pageStep;
        if(release_y - press_y != 0 && qAbs(release_y - press_y) > 45)
        {
            //qDebug()<<"obj->objectName()="<<obj->objectName();
            int mseconds = pressDateTime.msecsTo(QDateTime::currentDateTime());
//            qDebug()<<"mseconds="<<mseconds;

            int limit = 440;
            pageStep = 240;//scrollBarV->pageStep();
//            qDebug()<<"pageStep="<<pageStep;
            if(mseconds > limit)//滑动的时间大于某个值的时候，不再滚动(通过增加分母)
            {
                mseconds = mseconds + (mseconds - limit) * 20;
            }

            if(release_y - press_y > 0)
            {
                endValue = m_scrollBarV->value()
                        - pageStep * (200.0 / mseconds);//.0避免避免强制转换为整形
                if(scrollV_min > endValue)
                {
                    endValue = scrollV_min;
                }
            }
            else if(release_y - press_y < 0)
            {
                    endValue = m_scrollBarV->value() + pageStep * (200.0 / mseconds);
                    if(endValue > scrollV_max)
                    {
                        endValue = scrollV_max;
                    }
            }
            if(mseconds > limit)
            {
                mseconds = 0;//滑动的时间大于某个值的时候，滚动距离变小，减小滑动的时间
            }
            animation->setDuration(mseconds+550);
            animation->setEndValue(endValue);
            animation->setEasingCurve(QEasingCurve::OutQuad);
            animation->start();
            return true;
        }
    }
    else if(event->type() == QEvent::MouseMove && move_y >= 0)
    {
        //窗口跟着鼠标移动

        int move_distance = QCursor::pos().y() - move_y;
        int endValue = m_scrollBarV->value() - move_distance;
        if(scrollV_min > endValue)
        {
            endValue = scrollV_min;
        }

        if(endValue > scrollV_max)
        {
            endValue = scrollV_max;
        }
        m_scrollBarV->setValue(endValue);
        //qDebug()<<"endValue="<<endValue;
        //qDebug()<<"move_distance="<<move_distance;
        move_y = QCursor::pos().y();
    }
    return false;
}



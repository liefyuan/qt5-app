#include "rollingbox.h"

RollingBox::RollingBox(QWidget *parent) :
    QWidget(parent),m_minRange(0),m_maxRange(7),//5640设置8种分辨率30fps采集、1080p是15fps
    m_currentValue(5),isDragging(0),m_deviation(0),m_numSize(18)
{
    homingAni  = new QPropertyAnimation(this,"deviation");
    homingAni->setDuration(300);
    homingAni->setEasingCurve(QEasingCurve::OutQuad);
}

void RollingBox::setRange(int min,int max)
{
    m_minRange = min;
    m_maxRange = max;

    if(m_currentValue < min)
        m_currentValue = min;
    if(m_currentValue > max)
        m_currentValue = max;

    //计算字符尺寸
    m_numSize = 3;
    int temp = m_maxRange;
    while(temp > 0)
    {
        temp /= 10;
        m_numSize++;
    }

    repaint();
}

int RollingBox::readValue()
{
    return m_currentValue;
}

void RollingBox::mousePressEvent(QMouseEvent *e)
{
    homingAni->stop();
    isDragging = 1;
    m_mouseSrcPos = e->pos().x();
}

void RollingBox::mouseMoveEvent(QMouseEvent *e)
{
    if(isDragging)
    {
        //数值到边界时，阻止继续往对应方向移动
        if((m_currentValue == m_minRange && e->pos().x() >= m_mouseSrcPos)||
                (m_currentValue == m_maxRange && e->pos().x() <= m_mouseSrcPos))
            return;

        m_deviation = e->pos().x() - m_mouseSrcPos;

        //若移动速度过快时进行限制
        if(m_deviation > (width()-1)/4)
            m_deviation = (width()-1)/4;
        else if(m_deviation < -(width()-1)/4)
            m_deviation = -(width()-1)/4;

        emit deviationChange((float)m_deviation/((width()-1)/4));

        repaint();
    }
}

void RollingBox::mouseReleaseEvent(QMouseEvent *)
{
    if(isDragging)
    {
        isDragging = 0;
        homing();
    }
}

void RollingBox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int Width = width()-1;
//    int Height = height()-1;

    if(m_deviation >= Width/4 && m_currentValue > m_minRange)
    {
        m_mouseSrcPos += Width/4;
        m_deviation -= Width/4;
        m_currentValue -= 1;
    }
    if(m_deviation <= -Width/4 && m_currentValue < m_maxRange)
    {
        m_mouseSrcPos -= Width/4;
        m_deviation += Width/4;
        m_currentValue += 1;
    }

    //中间数字
    paintNum(painter,m_currentValue,m_deviation);

    //两侧数字1
    if(m_currentValue != m_minRange)
        paintNum(painter,m_currentValue-1,m_deviation-Width/4);
    if(m_currentValue != m_maxRange)
        paintNum(painter,m_currentValue+1,m_deviation+Width/4);

    //两侧数字2,超出则不显示
    if(m_deviation >= 0 && m_currentValue-2 >= m_minRange)
        paintNum(painter,m_currentValue-2,m_deviation-Width/2);
    if(m_deviation <= 0 && m_currentValue+2 <= m_maxRange)
        paintNum(painter,m_currentValue+2,m_deviation+Width/2);

    //边框
//    painter.setPen(QPen(QColor(0,0,0,120),2));
//    painter.drawLine(Width/8*3,0,Width/8*3,Height);
//    painter.drawLine(Width/8*5.2,0,Width/8*5.2,Height);
}

void RollingBox::paintNum(QPainter &painter,int num,int deviation)
{
    QString  resolution_Ratio; // 分辨率大小
    int Width = width()-1;
    int Height = height()-1;

    int size = (Width - qAbs(deviation))/m_numSize;
    int transparency = 255-510*qAbs(deviation)/Width;
    int width = Width/2-3*qAbs(deviation)/4;
    int x = Width/2+deviation-width/2;

    QFont font;
    font.setPixelSize(size);
    painter.setFont(font);
    painter.setPen(QColor(255,255,255,transparency));
    switch (num) {
    case 0:
        resolution_Ratio = "176x144\n30fps";
        break;
    case 1:
        resolution_Ratio = "320x240\n30fps";
        break;
    case 2:
        resolution_Ratio = "640x480\n30fps";
        break;
    case 3:
        resolution_Ratio = "720x480\n30fps";
        break;
    case 4:
        resolution_Ratio = "720x576\n30fps";
        break;
    case 5:
        resolution_Ratio = "1024x768\n30fps";
        break;
    case 6:
        resolution_Ratio = "1280x720\n30fps";
        break;
    case 7:
        resolution_Ratio = "1920x1080\n15fps";
        break;
    default:
        break;
    }
    painter.drawText(QRectF(x,0,width,Height),
                     Qt::AlignCenter,
                     /*QString::number(num)*/resolution_Ratio);
}

void RollingBox::homing()
{
    //将数字矫正到中心
    if(m_deviation > width()/8)
    {
        homingAni->setStartValue((width()-1)/8-m_deviation);
        homingAni->setEndValue(0);
        m_currentValue--;
    }
    else if(m_deviation > -width()/8)
    {
        homingAni->setStartValue(m_deviation);
        homingAni->setEndValue(0);
    }
    else if(m_deviation < -width()/8)
    {
        homingAni->setStartValue(-(width()-1)/8-m_deviation);
        homingAni->setEndValue(0);
        m_currentValue++;
    }
    emit currentValueChanged(m_currentValue);
    homingAni->start();
}

int RollingBox::readDeviation()
{
    return m_deviation;
}

void RollingBox::setDeviation(int n)
{
    m_deviation = n;
    repaint();
}
void RollingBox::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0){
        if(m_currentValue >= m_maxRange)
        {
            return;
        }
        else
            m_currentValue+=1;
    }else{
        if(m_currentValue <= m_minRange)
        {
            return;
        }
        else
            m_currentValue-=1;
    }
    update();
    event->accept();
}




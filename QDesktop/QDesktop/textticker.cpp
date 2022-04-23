#include "textticker.h"
#include <QPainter>
#include <QTimer>
TextTicker::TextTicker(QWidget *parent)
    : QLabel(parent)
{
    setMinimumWidth((float)150/800*this->geometry().width());
    setMinimumHeight(50);
    m_curIndex = 0;//当前角码
    //m_showText = "This is a textTicker Text!";//显示的文字
    m_charWidth = fontMetrics().width("a");//每个字符的宽度
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TextTicker::updateIndex);
    timer->start(200);
}

// 通过set方法访问私有成员
void TextTicker::setSongNameText(QString songsName)
{
    m_showText = songsName;
}

TextTicker::~TextTicker()
{


}

void TextTicker::paintEvent(QPaintEvent *event)
{
    //this->paintEvent(event);
    QPainter painter(this);
    painter.drawText(-30, 30, m_showText.mid(m_curIndex));
    painter.drawText(width() - m_charWidth*m_curIndex, 30, m_showText.left(m_curIndex+50));
}


void TextTicker::updateIndex()
{
    update();
    m_curIndex++;
    if (m_curIndex*m_charWidth > width())
        m_curIndex = 0;
}

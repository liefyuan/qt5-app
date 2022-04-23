#ifndef TEXTTICKER_H
#define TEXTTICKER_H
/* 滚动字幕类参考网址，喝水思源，不忘出处
 https://blog.csdn.net/u011417605/article/details/51211853*/
#include <QtWidgets/QLabel>


class TextTicker : public QLabel
{
    Q_OBJECT


public:
    TextTicker(QWidget *parent = 0);
    ~TextTicker();
    void setSongNameText(QString songsName);


protected:
    void paintEvent(QPaintEvent *event);
    void updateIndex();


private:
    int m_charWidth;
    int m_curIndex;
    QString m_showText;
};
#endif // TEXTTICKER_H


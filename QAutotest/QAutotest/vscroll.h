#ifndef VSCROLL_H
#define VSCROLL_H
/**
* @projectName
* @brief         仿iso滚屏效果
* @author        Day
* @date          2019.11.20
*/
#include <QScrollBar>
#include <QPropertyAnimation>
#include <QDateTime>
#include <QTimer>
#include <QEvent>
/*喝水思源，不忘出处https://blog.csdn.net/lthcth111/article/details/52223950*/
#include <QWidget>

class Vscroll : public QWidget
{
    Q_OBJECT
public:
    explicit Vscroll(QWidget *parent = 0);
    void setObjectList(QWidget *obj);
    void setObjectScrollBar(QScrollBar *obj);

signals:

public slots:
private:
    QScrollBar *m_scrollBarV;
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // VSCROLL_H

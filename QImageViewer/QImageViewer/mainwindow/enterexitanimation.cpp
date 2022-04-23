#include "enterexitanimation.h"
#include <QPropertyAnimation>
#include <QProcess>
Enterexitanimation::Enterexitanimation(QWidget *parent) : QWidget(parent)
{
    closeBt = new QPushButton(this);
    setMaximumSize(QSize(48,48));
    closeBt->setGeometry(0,0,48,48);
    closeBt->setStyleSheet("QPushButton{border-image:url(:/images/menu);outline:none}"
                           "QPushButton:pressed{border-image:url(:/images/menu_clicked);}");
    connect(closeBt,SIGNAL(clicked()),this,SLOT(exit_annimation()));
}
void Enterexitanimation::setAnimationObject(QMainWindow *obj)
{
    animationObject = obj;
    /* 进场动画 */
    QPropertyAnimation *new_Animation = new QPropertyAnimation(animationObject, "geometry");
    new_Animation->setDuration(210);//动画长度是0.21s
    new_Animation->setEndValue(QRect(0, 0,animationObject->geometry().width(),
                                      animationObject->geometry().height())
                                  );
    new_Animation->setStartValue(QRect(0, animationObject->geometry().height(),
                                      0, 0));
    new_Animation->start();
    connect(new_Animation,SIGNAL(finished()),this,SLOT(slot_update()));

}

void Enterexitanimation::exit_annimation()
{
    /* 退场动画 */
    QPropertyAnimation *new_Animation = new QPropertyAnimation(animationObject, "geometry");
    new_Animation->setDuration(200);//动画长度是0.2s
    new_Animation->setStartValue(QRect(animationObject->geometry().x(), animationObject->geometry().y(),
                                        animationObject->geometry().width(), animationObject->geometry().height())
                                  );
    new_Animation->setEndValue(QRect(animationObject->geometry().width(),-animationObject->geometry().y(),
                                      animationObject->geometry().width(),animationObject->geometry().y()));
    new_Animation->start();
    connect(new_Animation,SIGNAL(finished()),this,SLOT(slot_close()));
}
void Enterexitanimation::slot_close()
{
    animationObject->close();
}
void Enterexitanimation::slot_update()
{
    animationObject->repaint();
}

#ifndef ENTEREXITANIMATION_H
#define ENTEREXITANIMATION_H
/**
* @projectName
* @brief         进退场动画与退出按钮
* @author        Day
* @date          2019.11.20
*/
#include <QPushButton>
#include <QWidget>
#include <QObject>
#include <QMainWindow>
class Enterexitanimation : public QWidget
{
    Q_OBJECT
public:
    explicit Enterexitanimation(QWidget *parent = 0);
    void setAnimationObject(QMainWindow *obj);

signals:

public slots:
    void slot_close();
    void exit_annimation();
    void slot_update();
private:
    QPushButton     *closeBt;
    QMainWindow     *animationObject;

};

#endif // ENTEREXITANIMATION_H

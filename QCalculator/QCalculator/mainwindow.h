#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"
#include <QMainWindow>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QFile>
#include <QtCore/qmath.h>
#include "calculatorpushbutton.h"
#include "enterexitanimation.h"
#include <QTimer>
#include <QMessageBox>


namespace NSOperator {


enum Operator {
    Addition,
    Subtraction,
    Multiplication,
    Division,
    Logarithm,
    Power,
    Square
};


}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotClickedPbInput(int number);
    void slotClickedPbMO(int number);
    void slotClickedPbClear(void);
    void slotClickedPbBackspace(void);
    void slotClickedPbEqu(void);
    void slotClickedPbBO(int number);

private:
    QLabel *labelDataDisplay;
    QVector<CalculatorPushButton *> pbInputArr;
    QVector<CalculatorPushButton *> pbMonocularOperatorArr;  // 单目运算符
    QVector<CalculatorPushButton *> pbBinocularOperatorArr;  // 双目运算符
    QPushButton *pbClear;
    QPushButton *pbBackspace;
    QPushButton *pbEqual;
    QWidget  *topWidget;
    QLabel   *appNameLabel;
    QPushButton *exitPushButton;
    Enterexitanimation *enterexitanimation;

    double firstOperand;          // 第一个操作数
    int currentOperate;           // 当前进行的运算操作
    bool flagSwitchOperand;       // 是否切换运算操作数
    bool flagSecondOperandReady;  // 第二个操作数准备好
    bool flagDataError;           // 操作数错误
    int LCD_Height;
    int LCD_Width;
    int btnWidth;
    int btnHeight;

    void inputPartBtnInit(void);
    void monocularOperatorBtnInit(void);
    void labelClearBtnInit(void);
    void backspaceBtnInit(void);
    void equalBtnInit(void);
    void binocularOperatorBtnInit(void);
};

#endif // MAINWINDOW_H

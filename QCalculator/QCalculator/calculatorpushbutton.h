#ifndef CALCULATORPUSHBUTTON_H
#define CALCULATORPUSHBUTTON_H

#include <QPushButton>


class CalculatorPushButton : public QPushButton
{
    Q_OBJECT

public:
    CalculatorPushButton(QWidget *parent = 0);
    int number;

signals:
    void signalClicked(int number);

private slots:
    void slotClicked(void);
};

#endif // CALCULATORPUSHBUTTON_H

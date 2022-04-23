#include "calculatorpushbutton.h"

CalculatorPushButton::CalculatorPushButton(QWidget *parent)
    : QPushButton(parent)
{
    this->number = -1;
    connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

void CalculatorPushButton::slotClicked(void) {
    emit signalClicked(this->number);
}

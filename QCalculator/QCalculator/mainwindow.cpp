#include "mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    /* 主窗口初始化 ，设置主窗体的大小 */
    this->setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height());
    this->setWindowFlags(Qt::FramelessWindowHint);      // 主窗口无边框
    this->setStyleSheet("QMainWindow {background-color: rgb(0, 0, 0);}");
    //this->setGeometry(0,0,800,480);
    QFont font;

    topWidget = new QWidget(this);
    topWidget->setGeometry(0,0,this->geometry().width(),40);
    topWidget->setStyleSheet("QWidget{background-color:rgba(30,30,30,100%)}");

    /* 显示label初始化 */
    labelDataDisplay = new QLabel("0", this);
    labelDataDisplay->setFocusPolicy(Qt::NoFocus);
    labelDataDisplay->setGeometry(0, 0, (float)780/800*this->geometry().width(),
                                  (float)155/480*this->geometry().height());
    labelDataDisplay->setAlignment(Qt::AlignRight | Qt::AlignBottom);


    /* 输入部分按钮初始化 */
    inputPartBtnInit();

    /* 单目运算符 */
    monocularOperatorBtnInit();

    /* 双目运算符 */
    binocularOperatorBtnInit();

    /* 清空显示 */
    labelClearBtnInit();

    /* 退格 */
    backspaceBtnInit();

    /* 等于 */
    equalBtnInit();

    /* 标志位复位 */
    currentOperate = -1;
    flagSwitchOperand = false;
    flagSecondOperandReady = false;
    flagDataError = false;

    /* 设置字体大小 */
    int pointSize        = (float)24/800*this->geometry().width();
    QString strPointSize = QString::number(pointSize);

    QString commomStyle = "QPushButton {color: rgb(255, 255, 255);"
                          "font: "+ strPointSize + "pt \"微软雅黑\";"
                                                   "background-color: rgb(50, 50, 50);}"
                                                   "QPushButton:pressed {background-color: rgb(218, 165, 32);}";

    for (int i = 0; i < 14; i++)
        pbInputArr[i]->setStyleSheet("QPushButton {color: rgb(255, 105, 180);"
                                     "font: "+ strPointSize + "pt \"微软雅黑\";"
                                                              "background-color: rgb(50, 50, 50);}"
                                                              "QPushButton:pressed {background-color: rgb(218, 165, 32);}");
    for (int i = 0; i < 8; i++)
        pbMonocularOperatorArr[i]->setStyleSheet(commomStyle);
    for (int i = 0; i < 7; i++)
        pbBinocularOperatorArr[i]->setStyleSheet(commomStyle);

    pbClear->setStyleSheet(commomStyle);
    pbBackspace->setStyleSheet(commomStyle);
    pbEqual->setStyleSheet(commomStyle);

    labelDataDisplay->setStyleSheet("QLabel { color: white;}");

    font.setPointSize((float)60/800*this->geometry().width());
    labelDataDisplay->setFont(font);

    enterexitanimation = new Enterexitanimation(this);
    enterexitanimation->setGeometry(this->geometry().width()-48,0,48,48); //设置位置
    enterexitanimation->setAnimationObject(this);//设置动画对象

}

MainWindow::~MainWindow()
{
    /* 按钮输入部分 */
    qDeleteAll(pbInputArr);
    pbInputArr.clear();

    /* 单目运算符 */
    qDeleteAll(pbMonocularOperatorArr);
    pbMonocularOperatorArr.clear();

    /* 双目运算符 */
    qDeleteAll(pbBinocularOperatorArr);
    pbBinocularOperatorArr.clear();

    /* 显示label */
    delete labelDataDisplay;

    /* 清零 退格 等于 */
    delete pbClear;
    delete pbBackspace;
    delete pbEqual;
}

void MainWindow::inputPartBtnInit(void) {
    pbInputArr.clear();

    for (int i = 0; i < 14; i++) {
        CalculatorPushButton *btn = new CalculatorPushButton(this);
        btn->setFocusPolicy(Qt::NoFocus);
        if (i < 10)
            btn->setText(QString::number(i));
        btn->number = i;
        connect(btn, SIGNAL(signalClicked(int)), SLOT(slotClickedPbInput(int)));
        pbInputArr.append(btn);
    }

    /* 数字 */
    pbInputArr[0]->setGeometry((float)400/800*this->geometry().width(), (float)400/480*this->geometry().height(),
                               (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbInputArr[1]->setGeometry((float)400/800*this->geometry().width(), (float)320/480*this->geometry().height(),
                               (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbInputArr[2]->setGeometry((float)500/800*this->geometry().width(), (float)320/480*this->geometry().height(),
                               (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbInputArr[3]->setGeometry((float)600/800*this->geometry().width(), (float)320/480*this->geometry().height(),
                               (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbInputArr[4]->setGeometry((float)400/800*this->geometry().width(), (float)240/480*this->geometry().height(),
                               (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbInputArr[5]->setGeometry((float)500/800*this->geometry().width(), (float)240/480*this->geometry().height(),
                               (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbInputArr[6]->setGeometry((float)600/800*this->geometry().width(), (float)240/480*this->geometry().height(),
                               (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbInputArr[7]->setGeometry((float)400/800*this->geometry().width(), (float)160/480*this->geometry().height(),
                               (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbInputArr[8]->setGeometry((float)500/800*this->geometry().width(), (float)160/480*this->geometry().height(),
                               (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbInputArr[9]->setGeometry((float)600/800*this->geometry().width(), (float)160/480*this->geometry().height(),
                               (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());

    /* 小数点 正负号 */
    pbInputArr[10]->setGeometry((float)500/800*this->geometry().width(), (float)400/480*this->geometry().height(),
                                (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbInputArr[11]->setGeometry((float)600/800*this->geometry().width(), (float)400/480*this->geometry().height(),
                                (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbInputArr[10]->setText(".");
    pbInputArr[11]->setText("+/-");

    /* π e */
    pbInputArr[12]->setGeometry(0, (float)160/480*this->geometry().height(),
                                (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbInputArr[13]->setGeometry((float)100/800*this->geometry().width(), (float)160/480*this->geometry().height(),
                                (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbInputArr[12]->setText("π");
    pbInputArr[13]->setText("e");
}

void MainWindow::slotClickedPbInput(int number) {
    if (flagDataError) {
        flagDataError = false;
        labelDataDisplay->setText("0");
    }

    /* 如果flagSwitchOperand标志位此时为真，则表示需要切换操作数 */
    if (flagSwitchOperand) {
        labelDataDisplay->setText("0");
        flagSwitchOperand = false;
        if (-1 != currentOperate)  // 如果
            flagSecondOperandReady = true;
    }

    QString currentText = labelDataDisplay->text();

    /* 这里需要分类进行处理: 数字、小数点、正负号、π、e */
    if (10 > number) {  //  数字部分
        if ( "0" == currentText )
            labelDataDisplay->setText(QString::number(number));
        else
            labelDataDisplay->setText(currentText + QString::number(number));
    }
    else if (10 == number) {  // 小数点
        if (currentText.contains('.'))
            return;

        labelDataDisplay->setText(currentText + ".");
    }
    else if (11 == number) {  // 正负号
        if ("0" == currentText)
            return;

        if (currentText.contains('-'))
            labelDataDisplay->setText(currentText.replace(0, 1, ""));
        else
            labelDataDisplay->setText("-" + currentText);
    }
    else {    // π  e
        flagSwitchOperand = true;
        if (12 == number)  // π
            labelDataDisplay->setText(QString::number(M_PI, 'g', 20));
        else
            labelDataDisplay->setText(QString::number(M_E, 'g', 20));
    }
}

void MainWindow::monocularOperatorBtnInit(void) {
    pbMonocularOperatorArr.clear();

    for (int i = 0; i < 8; i++) {
        CalculatorPushButton *btn = new CalculatorPushButton(this);
        btn->setFocusPolicy(Qt::NoFocus);
        btn->number = i;
        connect(btn, SIGNAL(signalClicked(int)), SLOT(slotClickedPbMO(int)));
        pbMonocularOperatorArr.append(btn);
    }

    pbMonocularOperatorArr[0]->setGeometry((float)700/800*this->geometry().width(), (float)400/480*this->geometry().height(),
                                           (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbMonocularOperatorArr[1]->setGeometry(0, (float)320/480*this->geometry().height(),
                                           (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbMonocularOperatorArr[2]->setGeometry((float)100/800*this->geometry().width(), (float)320/480*this->geometry().height(),
                                           (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbMonocularOperatorArr[3]->setGeometry((float)200/800*this->geometry().width(), (float)320/480*this->geometry().height(),
                                           (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbMonocularOperatorArr[4]->setGeometry((float)300/800*this->geometry().width(), (float)320/480*this->geometry().height(),
                                           (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbMonocularOperatorArr[5]->setGeometry(0, (float)240/480*this->geometry().height(),
                                           (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbMonocularOperatorArr[6]->setGeometry((float)100/800*this->geometry().width(), (float)240/480*this->geometry().height(),
                                           (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbMonocularOperatorArr[7]->setGeometry((float)200/800*this->geometry().width(), (float)240/480*this->geometry().height(),
                                           (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());

    pbMonocularOperatorArr[0]->setText("%");
    pbMonocularOperatorArr[1]->setText("bin");
    pbMonocularOperatorArr[2]->setText("oct");
    pbMonocularOperatorArr[3]->setText("dec");
    pbMonocularOperatorArr[4]->setText("hex");
    pbMonocularOperatorArr[5]->setText("sin");
    pbMonocularOperatorArr[6]->setText("cos");
    pbMonocularOperatorArr[7]->setText("tan");
}

void MainWindow::slotClickedPbMO(int number) {
    /* 如果出错  则不能进行任何的运算操作 */
    if (flagDataError)
        return;

    QString currentText = labelDataDisplay->text();
    int count = currentText.count();
    double data;

    /* 如果显示的数据最后带有一个小数点 则需要去掉 小数点后面没有数字没有意义 */
    if ('.' == currentText.at(count - 1))
        currentText = currentText.replace(count - 1, 1, "");

    /* 判断是否是一个浮点数 浮点数不支持进制转换 */
    if (currentText.contains('.') &&
            0 < number && 5 > number)
        return;

    /* 在进行操作之前需将其他进制数据统一转换为十进制 */
    data = currentText.toDouble();  // 十进制

    if (currentText.contains("0b"))  // 二进制表示的数据
        data = currentText.replace(0, 2, "").toLongLong(0, 2);

    if (currentText.contains("0o"))  // 八进制
        data = currentText.replace(0, 2, "").toLongLong(0, 8);

    if (currentText.contains("0x"))  // 十六进制
        data = currentText.replace(0, 2, "").toLongLong(0, 16);

    /* 进制数转换 */
    switch (number) {
    case 0:   // %
        labelDataDisplay->setText(QString::number(data/100, 'g', 10));
        break;

    case 1:   // bin
        labelDataDisplay->setText(QString("0b%1").arg((long long)data, 0, 2));
        break;

    case 2:   // oct
        labelDataDisplay->setText(QString("0o%1").arg((long long)data, 0, 8));
        break;

    case 3:  // dec
        labelDataDisplay->setText(QString::number(data, 'g', 10));
        break;

    case 4:  // hex
        labelDataDisplay->setText(QString("0x%1").arg((long long)data, 0, 16));
        break;

    case 5:   // sin
        labelDataDisplay->setText(QString::number(qSin(data), 'g', 10));
        break;

    case 6:   // cos
        labelDataDisplay->setText(QString::number(qCos(data), 'g', 10));
        break;

    case 7:
        labelDataDisplay->setText(QString::number(qTan(data), 'g', 10));
        break;
    }

    /* 标志位置位 */
    currentOperate = -1;
    flagSwitchOperand = true;
    flagSecondOperandReady = false;
}

void MainWindow::labelClearBtnInit(void) {
    pbClear = new QPushButton(this);
    pbClear->setFocusPolicy(Qt::NoFocus);
    pbClear->setGeometry((float)700/800*this->geometry().width(), (float)160/480*this->geometry().height(),
                         (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbClear->setText("C");
    connect(pbClear, SIGNAL(clicked()), this, SLOT(slotClickedPbClear()));
}

void MainWindow::slotClickedPbClear(void) {
    labelDataDisplay->setText("0");
    currentOperate = -1;
    flagSwitchOperand = false;
    flagSecondOperandReady = false;
    flagDataError = false;
}

void MainWindow::backspaceBtnInit(void) {
    pbBackspace = new QPushButton(this);
    pbBackspace->setFocusPolicy(Qt::NoFocus);
    pbBackspace->setGeometry((float)700/800*this->geometry().width(), (float)240/480*this->geometry().height(),
                             (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbBackspace->setText("DEL");
    connect(pbBackspace, SIGNAL(clicked()), this, SLOT(slotClickedPbBackspace()));
}

void MainWindow::slotClickedPbBackspace(void) {
    if (flagSwitchOperand ||
            flagDataError)
        return;

    QString currentText = labelDataDisplay->text();
    int count = currentText.count();

    /* 如果显示上已经是最后一个数字了 则退格就是清零了 显示清零  并非状态复位 */
    if (1 >= count) {
        labelDataDisplay->setText("0");
        return;
    }

    /* 如果此数字是一个负数 又该如何退格 */
    currentText = currentText.replace(count - 1, 1, "");
    if ("-" == currentText)
        labelDataDisplay->setText("0");
    else
        labelDataDisplay->setText(currentText);
}

void MainWindow::equalBtnInit(void) {
    pbEqual = new QPushButton(this);
    pbEqual->setFocusPolicy(Qt::NoFocus);
    pbEqual->setGeometry((float)700/800*this->geometry().width(), (float)320/480*this->geometry().height(),
                         (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbEqual->setText("=");
    connect(pbEqual, SIGNAL(clicked()), this, SLOT(slotClickedPbEqu()));
}

void MainWindow::slotClickedPbEqu(void) {
    /* 只针对双目运算符 第二个操作数没准备好 直接退出
     * 如果是出错的情况下 不能执行任何的运算操作
     */
    if (!flagSecondOperandReady ||
            flagDataError ||
            (-1 == currentOperate) )
        return;
    flagSecondOperandReady = false;

    QString currentText = labelDataDisplay->text();
    int count = currentText.count();
    double data;

    /* 判断显示的数据最后一个字符是不是小数点 如果是则去掉 */
    if ('.' == currentText.at(count - 1))
        data = currentText.replace(count - 1, 1, "").toDouble();
    else
        data = currentText.toDouble();
    /* 执行等于的时候处理的都是十进制数据 所以不需要再进制数转化 */

    switch (currentOperate) {
    case NSOperator::Addition:   // 加法
        labelDataDisplay->setText(QString::number(firstOperand + data, 'g', 10));
        break;

    case NSOperator::Subtraction: // 减法
        labelDataDisplay->setText(QString::number(firstOperand - data, 'g', 10));
        break;

    case NSOperator::Multiplication: // 乘法
        labelDataDisplay->setText(QString::number(firstOperand * data, 'g', 10));
        break;

    case NSOperator::Division:  //  除法
        if (0 == data) {
            labelDataDisplay->setText("输入无效!");
            flagDataError = true;
            currentOperate = -1;
            flagSwitchOperand = false;
            return;
        }
        labelDataDisplay->setText(QString::number(firstOperand / data, 'g', 10));
        break;

    case NSOperator::Logarithm: { // log指数
        double tmp = qLn(firstOperand)/qLn(data);
        if ( std::isnan(tmp) ) {
            labelDataDisplay->setText("输入无效!");
            flagDataError = true;
            currentOperate = -1;
            flagSwitchOperand = false;
            return;
        }
        labelDataDisplay->setText(QString::number(tmp, 'g', 10));
    }
        break;

    case NSOperator::Power: {  //  幂
        double tmp = qPow(firstOperand, data);
        if ( std::isnan(tmp) ) {
            labelDataDisplay->setText("输入无效!");
            flagDataError = true;
            currentOperate = -1;
            flagSwitchOperand = false;
            return;
        }
        labelDataDisplay->setText(QString::number(tmp, 'g', 10));
    }
        break;

    case NSOperator::Square: {  // 开方
        double tmp = qPow(firstOperand, 1/data);
        if ( std::isnan(tmp) ) {
            labelDataDisplay->setText("输入无效!");
            flagDataError = true;
            currentOperate = -1;
            flagSwitchOperand = false;
            return;
        }
        labelDataDisplay->setText(QString::number(tmp, 'g', 10));
    }
        break;
    }

    currentOperate = -1;           // 将当前的操作置空
    flagSwitchOperand = true;      // 这个就不用说了
    return;
}

void MainWindow::binocularOperatorBtnInit(void) {
    pbBinocularOperatorArr.clear();

    for (int i = 0; i < 7; i++) {
        CalculatorPushButton *btn = new CalculatorPushButton(this);
        btn->number = i;
        btn->setFocusPolicy(Qt::NoFocus);
        connect(btn, SIGNAL(signalClicked(int)), this, SLOT(slotClickedPbBO(int)));
        pbBinocularOperatorArr.append(btn);
    }

    pbBinocularOperatorArr[0]->setGeometry(0, (float)400/480*this->geometry().height(),
                                           (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbBinocularOperatorArr[1]->setGeometry((float)100/800*this->geometry().width(), (float)400/480*this->geometry().height(),
                                           (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbBinocularOperatorArr[2]->setGeometry((float)200/800*this->geometry().width(), (float)400/480*this->geometry().height(),
                                           (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbBinocularOperatorArr[3]->setGeometry((float)300/800*this->geometry().width(), (float)400/480*this->geometry().height(),
                                           (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbBinocularOperatorArr[4]->setGeometry((float)300/800*this->geometry().width(), (float)240/480*this->geometry().height(),
                                           (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbBinocularOperatorArr[5]->setGeometry((float)200/800*this->geometry().width(), (float)160/480*this->geometry().height(),
                                           (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());
    pbBinocularOperatorArr[6]->setGeometry((float)300/800*this->geometry().width(), (float)160/480*this->geometry().height(),
                                           (float)100/800*this->geometry().width(), (float)80/480*this->geometry().height());

    pbBinocularOperatorArr[0]->setText("+");
    pbBinocularOperatorArr[1]->setText("-");
    pbBinocularOperatorArr[2]->setText("*");
    pbBinocularOperatorArr[3]->setText("/");
    pbBinocularOperatorArr[4]->setText("log");
    pbBinocularOperatorArr[5]->setText("x^y");
    pbBinocularOperatorArr[6]->setText("√");
}


void MainWindow::slotClickedPbBO(int number) {
    /* 出错情况下 禁止任何运算操作 */
    if (flagDataError)
        return;

    QString currentText = labelDataDisplay->text();
    int count = currentText.count();
    double data;

    /* 处理当前字符串最后一个是否是 小数点 小数点后面没数字多尴尬! */
    if ('.' == currentText.at(count - 1))
        data = currentText.replace(count - 1, 1, "").toDouble();
    else
        data = currentText.toDouble();

    /* 转换为十进制数据 */
    if (currentText.contains("0b"))
        data = currentText.replace(0, 2, "").toLongLong(0, 2);

    if (currentText.contains("0o"))
        data = currentText.replace(0, 2, "").toLongLong(0, 8);

    if (currentText.contains("0x"))
        data = currentText.replace(0, 2, "").toLongLong(0, 16);

    /* 如果此前没有进行运算操作 */
    if ( -1 == currentOperate ) {
        currentOperate = number;
        firstOperand = data;
        flagSwitchOperand = true;
        flagSecondOperandReady = false;
        labelDataDisplay->setText(QString::number(data));
        return;
    }

    /* 如果第二个操作数没住备好 则此次按下运算符 我认为是你是需要替换
     * 当前的运算操作
     */
    if (!flagSecondOperandReady) {
        currentOperate = number;
        return;
    }
    flagSecondOperandReady = false;

    /* 运算 */
    switch (currentOperate) {
    case NSOperator::Addition:
        labelDataDisplay->setText(QString::number(firstOperand + data, 'g', 10));
        firstOperand += data;
        break;

    case NSOperator::Subtraction:
        labelDataDisplay->setText(QString::number(firstOperand - data, 'g', 10));
        firstOperand -= data;
        break;

    case NSOperator::Multiplication:
        labelDataDisplay->setText(QString::number(firstOperand * data, 'g', 10));
        firstOperand *= data;
        break;

    case NSOperator::Division: {
        if (0 == data) {
            labelDataDisplay->setText("输入无效!");
            flagDataError = true;
            currentOperate = -1;
            flagSwitchOperand = false;
            return;
        }
        labelDataDisplay->setText(QString::number(firstOperand / data, 'g', 10));
        firstOperand /= data;
    }
        break;

    case NSOperator::Logarithm: {
        double tmp = qLn(firstOperand)/qLn(data);
        if ( std::isnan(tmp) ) {
            labelDataDisplay->setText("输入无效!");
            flagDataError = true;
            currentOperate = -1;
            flagSwitchOperand = false;
            return;
        }
        labelDataDisplay->setText(QString::number(tmp, 'g', 10));
        firstOperand = tmp;
    }
        break;

    case NSOperator::Power: {
        double tmp = qPow(firstOperand, data);
        if ( std::isnan(tmp) ) {
            labelDataDisplay->setText("输入无效!");
            flagDataError = true;
            currentOperate = -1;
            flagSwitchOperand = false;
            return;
        }
        labelDataDisplay->setText(QString::number(tmp, 'g', 10));
        firstOperand = tmp;
    }
        break;

    case NSOperator::Square: {
        double tmp = qPow(firstOperand, 1/data);
        if ( std::isnan(tmp) ) {
            labelDataDisplay->setText("输入无效!");
            flagDataError = true;
            currentOperate = -1;
            flagSwitchOperand = false;
            return;
        }
        labelDataDisplay->setText(QString::number(tmp, 'g', 10));
        firstOperand = tmp;
    }
        break;
    }

    flagSwitchOperand = true;
    currentOperate = number;
}




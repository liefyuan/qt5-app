#include "mainwindow.h"
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 主窗口设置 */
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->installEventFilter(this);
    this->setStyleSheet("QMainWindow {background-color: rgb(50, 50, 50)}");
    this->setGeometry(0,0,QApplication::desktop()->screenGeometry().width(),
                      QApplication::desktop()->screenGeometry().height());
    //this->setGeometry(0,0,480,272);

    count = 0;

    /* label初始化 */
    labelTitle = new QLabel(this);
    labelCurrentTemp = new QLabel(this);
    labelTempUpdate = new QLabel(this);

    labelTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labelCurrentTemp->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labelTempUpdate->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    labelTitle->setText("芯片内部温度采集系统");
    labelTitle->setAlignment(Qt::AlignCenter);
    labelCurrentTemp->setText("当前温度");
    labelTempUpdate->setText("0");

    /* process初始化 */
    process = new QProcess(this);

    /* 定时器初始化 */
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimeoutTimer()));
    timer->start(200);

    /* 功能性按钮设置 */
    pbStop = new QPushButton(this);
    pbReset = new QPushButton(this);
    pbReset->setStyleSheet(PBRESET_STYLE);
    pbStop->setStyleSheet(PBSTOP_STYLE);
    connect(pbStop, SIGNAL(clicked()), this, SLOT(slotClickedPbStop()));
    connect(pbReset, SIGNAL(clicked()), this, SLOT(slotClickedPbReset()));

    labelTitle->setGeometry((float)10/800*this->geometry().width(), 0,
                            this->geometry().width(), (float)40/480*this->geometry().height());

    labelTitle->setStyleSheet("QLabel {color: white;}");
    labelCurrentTemp->setStyleSheet("QLabel {color: white;}");
    labelTempUpdate->setStyleSheet("QLabel {color: white;}");

    QFont font;
    font.setPixelSize((float)28/800*this->geometry().width());
    labelTitle->setFont(font);
    font.setPixelSize((float)23/800*this->geometry().width());
    labelCurrentTemp->setFont(font);
    font.setPixelSize((float)18/800*this->geometry().width());
    labelTempUpdate->setFont(font);


    labelCurrentTemp->setGeometry((float)700/800*this->geometry().width(), (float)80/480*this->geometry().height(),
                                  (float)100/800*this->geometry().width(), (float)40/480*this->geometry().height());
    labelTempUpdate->setGeometry((float)690/800*this->geometry().width(), (float)120/480*this->geometry().height(),
                                 (float)100/800*this->geometry().width(), (float)30/480*this->geometry().height());
    pbStop->setGeometry((float)705/800*this->geometry().width(), (float)330/480*this->geometry().height(),
                        (float)80/800*this->geometry().width(), (float)40/480*this->geometry().height());
    pbReset->setGeometry((float)705/800*this->geometry().width(), (float)390/480*this->geometry().height(),
                         (float)80/800*this->geometry().width(), (float)40/480*this->geometry().height());

    /* 画图初始化设置 */
    customPlotInit();

    enterexitanimation = new Enterexitanimation(this);
    enterexitanimation->setGeometry(this->geometry().width()-48,0,48,48); //设置位置
    enterexitanimation->setAnimationObject(this);//设置动画对象
}

MainWindow::~MainWindow()
{

}


void MainWindow::customPlotInit(void) {
    /* 实例化，设置位置、背景颜色 */
    QBrush brush(QColor(50,50,50));
    customPlot = new QCustomPlot(this);
    customPlot->setGeometry(0, (float)40/480*this->geometry().height(),
                            (float)700/800*this->geometry().width(), (float)440/480*this->geometry().height());
    customPlot->setBackground(brush);
    customPlot->installEventFilter(this);

    /* x轴、Y轴相关配置 */
    QPen pen(QColor(Qt::white));
    QFont font;
    font.setPointSize((float)16/800*this->geometry().width());
    customPlot->xAxis->setLabelColor(QColor(Qt::white)); // X轴上标识label字体颜色
    customPlot->yAxis->setLabelColor(QColor(Qt::white));
    customPlot->xAxis->setTickPen(pen);                  //  设置x轴上坐标点上对应的刻度线的颜色
    customPlot->yAxis->setTickPen(pen);
    customPlot->xAxis->setBasePen(pen);                  //  设置x轴 轴线本身的颜色
    customPlot->yAxis->setBasePen(pen);
    customPlot->xAxis->setTickLabelColor(QColor(Qt::white)); // 设置x轴刻度值文本的颜色
    customPlot->yAxis->setTickLabelColor(QColor(Qt::white));
    customPlot->xAxis->setSubTicks(false);  //  隐藏x轴刻度线
    customPlot->yAxis->setSubTicks(false);  //  隐藏y轴刻度线
    customPlot->xAxis->setLabelFont(font);  //  设置x轴标识label文本字体大小
    customPlot->yAxis->setLabelFont(font);  //  设置y轴标识label文本字体大小
    font.setPointSize((float)14/800*this->geometry().width());
    customPlot->xAxis->setTickLabelFont(font);
    customPlot->yAxis->setTickLabelFont(font);
    customPlot->xAxis->setLabel("时间/200ms");
    customPlot->yAxis->setLabel("温度/摄氏度");
    customPlot->xAxis->setRange(AXIS_X_MIN, AXIS_X_MAX);   //  设置x轴数据范围
    customPlot->yAxis->setRange(AXIS_Y_MIN, AXIS_Y_MAX);   //  设置y轴数据范围

    /* 增加一个数据曲线 */
    brush.setColor(QColor(73, 147, 221));  // 设置笔刷的颜色
    pen.setColor(QColor(73, 147, 221));    // 设置画笔的颜色
    customPlot->addGraph();                // 增加曲线图
    customPlot->graph(0)->setName("Temp"); // 设置曲线的名字
    customPlot->graph(0)->setBrush(brush); // 设置曲线下方的背景颜色
    customPlot->graph(0)->setPen(pen);     // 设置曲线画笔的颜色
    customPlot->graph(0)->setLineStyle(QCPGraph::lsLine); // 设置连接线的类型 两点直线连接

    /* 刷新显示 */
    customPlot->replot();
}


void MainWindow::slotTimeoutTimer(void) {

    /* count计数判断 */
    if (COUNT_MAX < count)
        count = 0;

    /* 读取温度传感器数据 */
    process->start(TEMP_INPUT_PATH);
    process->waitForFinished(-1);
    QString outPut = QString(process->readAllStandardOutput());
    double temp = outPut.toDouble() / 1000;

    /* 更新X轴 */
    if (count > AXIS_X_MAX)
        customPlot->xAxis->setRange(count - AXIS_X_MAX, count);

    /* 添加数据 */
    customPlot->graph(0)->addData(count, temp);
    labelTempUpdate->setText(QString::number(temp));
    customPlot->replot();

    /* 更新计数 */
    count++;
}


void MainWindow::slotClickedPbStop(void) {
    if (timer->isActive()) {
        timer->stop();
        pbStop->setStyleSheet(PBSTART_STYLE);
    }
    else {
        timer->start(200);
        pbStop->setStyleSheet(PBSTOP_STYLE);
    }
}

void MainWindow::slotClickedPbReset(void) {
    count = 0;
    customPlot->graph(0)->data().data()->clear();
    customPlot->xAxis->setRange(0, AXIS_X_MAX);
    customPlot->replot();
    labelTempUpdate->setText("0");
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
//无
    return QWidget::eventFilter(watched,event);//将事件传递给父类
}


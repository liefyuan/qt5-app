#include "mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 主窗口设置 */
    this->setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height());
    //this->setGeometry(0,0,800,480);
    this->setStyleSheet("QMainWindow{ background-color:white }");   // 主窗口背景
    this->setWindowFlags(Qt::FramelessWindowHint);                  // 无边框处理
    this->installEventFilter(this);

    /* 右边模块初始化 */
    parent_widget = new QWidget(this);
    parent_widget->setGeometry((float)640/800*this->geometry().width(), 0,
                               (float)160/800*this->geometry().width(),
                               this->geometry().height());
    parent_widget->setStyleSheet("QWidget{ background-color:#FF4385; }");

    QFont font;
    font.setPixelSize((float)18/800*this->geometry().width());

    today_date = QDate::currentDate();   // 获取今天对应的日期

    /* 初始化日历控件 */
    calendar_widget = new QCalendarWidget(this);
    calendar_widget->setGeometry(0, (float)96/480*this->geometry().height(),
                                 (float)640/800*this->geometry().width(),
                                 (float)384/480*this->geometry().height());
    calendar_widget->setNavigationBarVisible(false);    // 不显示导航栏
    calendar_widget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader); // 不显示最左边的周期统计列表
    calendar_widget->setFirstDayOfWeek(Qt::Sunday);     // 将周日显示在第一列
    calendar_widget->setFocusPolicy(Qt::NoFocus);       // 去除虚线框（焦点框）

    font.setPixelSize((float)26/800*this->geometry().width());
    calendar_widget->setFont(font);                     // 设置日历上的字体大小

    format.setForeground(QBrush(QColor("#FF4385")));
    format.setBackground(QBrush(QColor(255, 255, 255)));
    calendar_widget->setHeaderTextFormat(format);  // 这里只能设置周一至周五的字体颜色（也就是前景色），周六周日不能设置

    format = calendar_widget->weekdayTextFormat(Qt::Monday);
    calendar_widget->setWeekdayTextFormat(Qt::Saturday, format);  // 让周六周日的颜色与其他星期相同
    calendar_widget->setWeekdayTextFormat(Qt::Sunday, format);

    format.setForeground(QBrush(QColor(255, 255, 255)));
    format.setBackground(QBrush(QColor("#FF4385")));
    calendar_widget->setDateTextFormat(today_date, format);  // 将今天的日期对应的颜色标识出来
    connect(calendar_widget, SIGNAL(currentPageChanged(int,int)), this, SLOT(slot_currentPageChanged(int, int)));


    today = new QLabel(parent_widget);
    today->setGeometry(0, 0, (float)160/800*this->geometry().width(),
                       (float)96/480*this->geometry().height());
    font.setPixelSize((float)50/800*this->geometry().width());
    today->setFont(font);
    today->setText("Today");
    today->setStyleSheet("QLabel{ color:rgb(255, 255, 255); }");
    today->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);   //设置对其方式为水平居中 底部对其

    year_month = new QLabel(parent_widget);
    year_month->setGeometry(0, (float)98/480*this->geometry().height(),
                            (float)160/800*this->geometry().width(),
                            (float)48/480*this->geometry().height());
    year_month->setStyleSheet("QLabel{ color:rgb(255, 255, 255); }");
    year_month->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    font.setPixelSize((float)22/800*this->geometry().width());
    year_month->setFont(font);
    year_month->setText(today_date.toString("yyyy年MM月"));

    day = new QLabel(parent_widget);
    day->setGeometry(0, (float)155/480*this->geometry().height(),
                     (float)160/800*this->geometry().width(),
                     (float)110/480*this->geometry().height());
    font.setPixelSize((float)102/800*this->geometry().width());
    day->setFont(font);
    day->setStyleSheet("QLabel{ color:rgb(255, 255, 255); }");
    day->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    day->setText(today_date.toString("dd"));

    week = new QLabel(parent_widget);
    week->setGeometry(0, (float)240/480*this->geometry().height(),
                      (float)160/800*this->geometry().width(),
                      (float)48/480*this->geometry().height());
    week->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    font.setPixelSize((float)22/800*this->geometry().width());
    week->setFont(font);
    week->setStyleSheet("QLabel{ color:rgb(255, 255, 255); }");
    week->setText(today_date.toString("dddd"));

    /* 左边可变年月 */
    switch_year_month = new QLabel(this);
    switch_year_month->setGeometry((float)40/800*this->geometry().width(),
                                   (float)35/480*this->geometry().height(),
                                   (float)170/800*this->geometry().width(),
                                   (float)48/480*this->geometry().height());
    switch_year_month->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    switch_year_month->setStyleSheet("QLabel{ color:#FF4385; }");
    font.setPixelSize((float)32/800*this->geometry().width());
    switch_year_month->setFont(font);
    switch_year_month->setText(today_date.toString("yyyy年M月"));

    /* 按钮初始化 */
    prev_month = new QPushButton(this);
    next_month = new QPushButton(this);
    prev_month->setFocusPolicy(Qt::NoFocus);
    next_month->setFocusPolicy(Qt::NoFocus);

    prev_month->setGeometry((float)480/800*this->geometry().width(),
                            (float)50/480*this->geometry().height(),
                            (float)37/800*this->geometry().width(),
                            (float)37/800*this->geometry().width());
    next_month->setGeometry((float)560/800*this->geometry().width(),
                            (float)50/480*this->geometry().height(),
                            (float)37/800*this->geometry().width(),
                            (float)37/800*this->geometry().width());
    prev_month->setStyleSheet("QPushButton{ border-image:url(:/icon/Resource/icon/prev.png); }"
                              "QPushButton:pressed {border-image: url(:/icon/Resource/icon/prev_1.png); }");
    next_month->setStyleSheet("QPushButton{ border-image:url(:/icon/Resource/icon/next.png); }"
                              "QPushButton:pressed {border-image: url(:/icon/Resource/icon/next_1.png); }");
    connect(prev_month, SIGNAL(clicked()), calendar_widget, SLOT(showPreviousMonth()));
    connect(next_month, SIGNAL(clicked()), calendar_widget, SLOT(showNextMonth()));

    /* 定时器初始化 */
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_timeout(void)));

    enterexitanimation = new Enterexitanimation(this);
    enterexitanimation->setGeometry(this->geometry().width()-48,0,48,48); //设置位置
    enterexitanimation->setAnimationObject(this);//设置动画对象
}

MainWindow::~MainWindow()
{

}

void MainWindow::slot_timeout(void)
{
    QDate date = QDate::currentDate();
    int temp1 = date.year();
    int temp2 = date.month();
    int temp3 = date.day();

    if ((temp1 != today_date.year()) ||
            temp2 != today_date.month() ||
            temp3 != today_date.day()) {
        calendar_widget->setCurrentPage(temp1, temp2);
        calendar_widget->setDateTextFormat(date, format);
        year_month->setText(date.toString("yyyy年MM月"));
        day->setText(date.toString("dd"));
        week->setText(date.toString("dddd"));
        today_date = date;
    }
}

void MainWindow::slot_currentPageChanged(int year, int month)
{
    QString str = QString::number(year, 10) + "年" + QString::number(month, 10) + "月";
    switch_year_month->setText(str);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
//无
    return QWidget::eventFilter(watched,event);// 将事件传递给父类
}

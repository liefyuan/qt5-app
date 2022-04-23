#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qcustomplot.h"
#include <QProcess>
#include "config.h"
#include <QMessageBox>
#include "enterexitanimation.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotTimeoutTimer(void);
    void slotClickedPbReset(void);
    void slotClickedPbStop(void);

private:
    QCustomPlot *customPlot;
    QTimer *timer;

    QProcess *process;

    QLabel *labelTitle;
    QLabel *labelCurrentTemp;
    QLabel *labelTempUpdate;

    QPushButton *pbStop;
    QPushButton *pbReset;
    unsigned int count;

    Enterexitanimation *enterexitanimation;

    void customPlotInit(void);
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // MAINWINDOW_H

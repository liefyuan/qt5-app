#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <mainwindow.h>
#include <QDebug>
#include <QElapsedTimer>
#include <QDateTime>
#include <QTextCodec>
#include <QMovie>
int main(int argc, char *argv[])
{
    /* 参考https://blog.csdn.net/chenlong12580/article/details/23713025 */
    QApplication app(argc, argv);
    /* 设置编码格式为utf-8 默认的情况下 无法识别中文路径 所以这里必须要设置为utf-8 */
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    /* 设置程序的工作目录为可执行文件所在目录 */
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    MainWindow window;
    window.show();

    return app.exec();
}


#include "qfilechose.h"
#include "QStyleFactory"
#include <QDebug>
#include <QApplication>
#include <QTreeView>
#include <QListView>
#include <QDialogButtonBox>
#include <QPushButton>
qFileChose::qFileChose(QWidget *parent)
    : QFileDialog(parent)
{

}
QString qFileChose::choseOpenFileName(QWidget *parent,QString caption,QString dir,QString filter,Options options)
{
    //注意，这里只是重写了它的大小，与滚动条的样式，其他没有做太多修改
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("QWidget{background-color:#FFFAFA}"
                        "QScrollBar:vertical {width:8px;background-color:transparent;}"\
                        "QScrollBar::handle:vertical {width:10px; background:rgba(0, 186, 255,80%); border-radius:4px;}"\
                        "QScrollBar::add-line:vertical {width:0px; height:0px;}"\
                        "QScrollBar::sub-line:vertical {width:0px; height:0px;}"\
                        "QScrollBar::handle:vertical:hover {width:8px; background:rgba(0, 186, 255,80%); border-radius:4px;}"\
                        "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical {background:transparent;}"\
                        "QScrollBar:horizontal {height:8px; background-color:transparent;}"\
                        "QScrollBar::handle:horizontal {height:10px; background-color:rgba(0, 186, 255,80%); border-radius:4px;}"\
                        "QScrollBar::add-line:horizontal {width:0px; height:0px;}"\
                        "QScrollBar::sub-line:horizontal {width:0px; height:0px;}"\
                        "QScrollBar::handle:horizontal:hover {height:8px; background:rgba(0, 186, 255,80%); border-radius:4px;}"\
                        "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal {background:transparent;}");//设置对话框的样式表

    if(parent->width()<800){
        this->setMaximumSize(parent->width(),parent->height());//重写它的大小
    }
    else{
        this->setMinimumSize(parent->width(),parent->height());//重写它的大小
    }

    this->setWindowTitle(caption);
    this->setDirectory(QDir(dir));
    this->setNameFilter(filter);
    this->setOptions(options);
    if(this->exec()== QDialog::Accepted)
        return this->selectedFiles().at(0);
    else
        return NULL;//点击取消时为空
}


qFileChose::~qFileChose()
{

}

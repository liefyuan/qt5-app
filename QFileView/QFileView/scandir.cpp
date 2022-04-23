#include "mainwindow.h"
/**
* @projectName
* @brief         扫描目录下的文件
* @author        Day
* @date          2019.11.20
*/
void MainWindow::scandir()
{
    QDir dir(USER_FILE_PATH);                                                       //定义dir目录
    arr.clear();                                                                    //清空容器
    fileViewListWidget->clear();                                                    //列表清空
    if (dir.exists()) {
        QStringList filter;                                                         //定义过滤器
        filter << "*";                                                              //包含所有文件/夹
        struct FileObjectInfo info;                                                 //结构体对象info
        // 获取该目录下的所有文件，再过滤，除.和..目录，.和..为linux下的当前目录与上一级目录
        QFileInfoList files = dir.entryInfoList(filter, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < files.count(); i++) {
            info.fileName = files.at(i).fileName();                             //文件名
            info.filePath = files.at(i).filePath();                             //路径名
            info.fileDate = files.at(i).lastModified().toString();              //日期
            if (files.at(i).isDir()){
                info.fileSuffix = "dir";//直接将文件类给予目录
            }else{
                info.fileSuffix = files.at(i).suffix();                         //获取文后缀名
            }
            info.fileSize = -1;                                                 //在这里先给fileSize赋一个值
            arr.append(info);                                                   //把获得的文件名与表格填进数组

            QListWidgetItem *item = new QListWidgetItem;                        //用于接收列表子项，再填进fileViewListWidget
            item->setSizeHint(QSize((float)100/800*this->geometry().width(),
                                    (float)100/800*this->geometry().width())    //设置每次个item的大小
                              );

            item->setText(info.fileName);

            if(files.at(i).isDir()){
                item->setIcon(QIcon(":/icon/Resource/icon/folder.png"));//设置列表所要显示类型的图标，下同
            }else {
                QString str = ":/icon/Resource/icon/";
                item->setIcon(QIcon(str + files.at(i).suffix()));

                if(files.at(i).suffix() == "mp4" || files.at(i).suffix() == "avi" || files.at(i).suffix() == "mkv"){
                    item->setIcon(QIcon(":/icon/Resource/icon/movie.png"));
                }

                if(files.at(i).isExecutable() == true){//判断这个文件是否为可执行文件，如果是，就显示可执行图标，否则显示普通文本图标
                    item->setIcon(QIcon(":/icon/Resource/icon/exec.png"));
                }else{
                    item->setIcon(QIcon(":/icon/Resource/icon/paper.png"));
                }
            }
            fileViewListWidget->addItem(item);//把item填进去表格
        }
    }
}

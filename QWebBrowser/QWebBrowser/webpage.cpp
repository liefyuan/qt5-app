#include "webpage.h"
 webPage::webPage(QObject *parent) :
    QWebPage(parent)
{
}
bool webPage::acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type){
    if(type==0){//如果是用户点击
        if(frame!=mainFrame()){ //如果不是在本窗口的连接
            emit openLink(request.url());//发送打开新连接信号
        }else{
            emit loadLink(request.url());//发送点击连接信号
        }
    }
    return QWebPage::acceptNavigationRequest(frame, request, type);
}


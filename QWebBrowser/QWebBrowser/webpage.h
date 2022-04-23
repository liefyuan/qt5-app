#ifndef WEBPAGE_H
#define WEBPAGE_H
#include <QWebPage>
#include <QNetworkRequest>
 class webPage : public QWebPage
{
    Q_OBJECT
public:
    explicit webPage(QObject *parent = 0);
signals:
    void loadLink(const QUrl & url);
    void openLink(const QUrl & url);

 protected:
    bool acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type);//重写

 };

#endif // WEBPAGE_H

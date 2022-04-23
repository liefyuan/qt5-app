#include "mainwindow.h"

void MainWindow::networkAccessManagerInit(void) {
    /* 实例化 */
    NAManager = new QNetworkAccessManager(this);

//    /* 绑定URL */
//    QNetworkRequest  ipGetNRequest;
//    ipGetNRequest.setUrl(QUrl(IP_GET_API_URL));

//    /* get */
//    QNetworkReply *reply = NAManager->get(ipGetNRequest);

    /* 连接信号、槽函数 */
//    connect(reply, SIGNAL(finished()), this, SLOT(slotFinishedIpGetReply()));
    slotFinishedIpGetReply();
}

void MainWindow::slotFinishedIpGetReply(void)
{
//    QNetworkReply *reply = (QNetworkReply *)sender();    // 获取信号发送者
//    QTextCodec *codec = QTextCodec::codecForName("gbk"); // 该接口返回的字符串采用gbk编码
//    QString temp = codec->toUnicode(reply->readAll());   // 读取所有返回数据并将其转换为Unicode编码
//    reply->deleteLater();                                // 这个要加上 避免发生内存泄漏

//    if (temp.isEmpty() || temp.isNull()) {
//        cityPushButton->setText("定位失败");
//        return;
//    }

    /* 获取ip地址 发送请求 */
    QNetworkRequest  ipQueryNRequest;
//    ipQueryNRequest.setUrl(QUrl(IP_QUERY_API_URL + temp));
    ipQueryNRequest.setUrl(QUrl(IP_QUERY_API_URL));
    QNetworkReply *newReply = NAManager->get(ipQueryNRequest);
    connect(newReply, SIGNAL(finished()), this, SLOT(slotFinishedIpQueryReply()));
}

void MainWindow::slotFinishedIpQueryReply(void)
{
    QNetworkReply *reply = (QNetworkReply *)sender();
    QTextCodec *codec = QTextCodec::codecForName("gbk");
    QString temp = codec->toUnicode(reply->readAll());
    reply->deleteLater();

    /* 判断字符串是否为空 */
    if (temp.isEmpty() || temp.isNull()) {
        cityPushButton->setText("定位失败");
        return;
    }

    /* 提取city字段 */
    int length = temp.length(); // 得到字符串长度
    int startIndex = temp.indexOf("city:") + 6;  // city:广州市,
    int endIndex = -1;

    if (5 == startIndex) { // 表示不存在city字段 -1
        cityPushButton->setText("定位失败");
        return;
    }

    for (int i = startIndex; i < length; i++) {

        if (',' == temp.at(i)) {
            endIndex = i;
            break;
        }
    }

    if (-1 == endIndex) {
        cityPushButton->setText("定位失败");
        return;
    }

    /* 获取城市名 发送请求 */
    QString cityName = temp.mid(startIndex, endIndex - startIndex - 1);
    QNetworkRequest  weatherGetNRequest;
    weatherGetNRequest.setUrl(QUrl(WEATHER_GET_API_URL + cityName));
    QNetworkReply *newReply = NAManager->get(weatherGetNRequest);
    connect(newReply, SIGNAL(finished()), this, SLOT(slotFinishedWeatherGetReply()));
}

void MainWindow::slotFinishedWeatherGetReply(void)
{
    QNetworkReply *reply = (QNetworkReply *)sender();
    QString temp = reply->readAll();
    reply->deleteLater();

    if (temp.isEmpty() || temp.isNull() ) {
        cityPushButton->setText("定位失败");
        return;
    }
    /* 创建Json相关对象 */
    QJsonParseError error;
    QJsonObject jsonObj;
    QJsonDocument jsonDocument;
    QString data;

    /* 解析Jason字符串 获取天气预告信息 */
    jsonDocument = QJsonDocument::fromJson(temp.toUtf8(), &error);
    if (error.error == QJsonParseError::NoError) {

        if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
            cityPushButton->setText("定位失败");
        }else {
            if (jsonDocument.isObject()) {
                jsonObj = jsonDocument.object();
                jsonObj = jsonObj.value("data").toObject();

                /* 当前城市数据 */
                data = jsonObj.value("city").toString();
                cityPushButton->setText(data);

                /* 当前温度数据 */
                data = jsonObj.value("wendu").toString() + "℃ ";
                QString currentTemp = data;
                currentTempLabel->setText(currentTemp);
                QJsonArray forecast = jsonObj.value("forecast").toArray();

                for(int i = 0; i < 5; i++){

                    QJsonObject objectDate = forecast.at(i).toObject();

                    /* 当前日期
                     * 这里的日期有一位数和两位数的，判断后截取的文本才可以居中显示
                     */
                    if(objectDate.value("date").toString().length() == 6){
                        data = objectDate.value("date").toString().mid(3, 3) + "\n"+ objectDate.value("date").toString().mid(0, 3) ;
                    }else{
                        data = objectDate.value("date").toString().mid(2, 3) + "\n"+ objectDate.value("date").toString().mid(0, 2) ;
                    }

                    if(i == 0)
                        weekLabelArr[i]->setText(data +"(今天)");
                    else
                        weekLabelArr[i]->setText(data);

                    /* 当前最高温度与最低温度 */
                    QJsonObject object = forecast.at(i).toObject();
                    data = object.value("low").toString().mid(3, 3) + "~";
                    data += object.value("high").toString().mid(3, 3);
                    if(i == 0)
                        minMaxTempLabelArr[i]->setText(currentTemp + data);
                    else
                        minMaxTempLabelArr[i]->setText(data);

                    /* 天气类型:如大雨等 */
                    data = object.value("type").toString();

                    if(i == 0){
                        weatherTypeLabelArr[i]->setText(data + "(实时)");
                        currentWeather->setText(data);
                    }
                    else
                        weatherTypeLabelArr[i]->setText(data);


                    /* 这里判断本地是否存在获取的天气数据类型的图片，没有预定义的则显示未定义的图片 */
                    data = "./Resource/weatherIcon/" + data + ".png";
                    QFileInfo file(data);
                    if(file.exists() == false){
                        weatherIconLabelArr[i]->setStyleSheet("QLabel{border-image:url(./Resource/weatherIcon/未定义.png);background-color:transparent}");
                    }else {
                        weatherIconLabelArr[i]->setStyleSheet("QLabel{border-image:url("  + data + ");background-color:transparent}");
                        if(i==0) // 今天的天气类型
                        currentWeatherIcon->setStyleSheet("QLabel{border-image:url("  + data + ");background-color:transparent}");
                    }
                }
            }else {
                cityPushButton->setText("定位失败");
            }
        }
    }else{
        cityPushButton->setText("定位失败");
    }
}

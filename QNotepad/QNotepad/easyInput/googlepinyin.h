#ifndef GOOGLEPINYIN_H
#define GOOGLEPINYIN_H

#include <QObject>

class GooglePinYin : public QObject
{
    Q_OBJECT
public:
    explicit GooglePinYin(QObject *parent = 0);

private:
    bool ok;            //是否正常
    int maxInputLen;    //最大输入长度
    int maxOutputLen;   //最大输出长度

public slots:
    //打开输入法核心
    bool open(const QString &dbPath);
    //关闭输入法核心
    void close();
    //清空缓存
    void flush();
    //重置搜索
    void reset();

    //查找拼音对应的汉字,返回个数
    int select(const QString &pinyin);
    //获取指定索引的汉字
    QString getChinese(int index);

    //删除汉字
    int deleteSelect(int pos);
    //当前查找位置
    int getPosition();
};

#endif // GOOGLEPINYIN_H

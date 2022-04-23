#ifndef EASYINPUT_H
#define EASYINPUT_H

#include <QtGui>
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtWidgets>
#endif

#include "googlepinyin.h"

namespace Ui
{
    class easyInput;
}

class easyInput : public QWidget
{
    Q_OBJECT
public:
    //单例模式,保证一个程序只存在一个输入法实例对象
    static easyInput *Instance(QWidget *parent);
    explicit easyInput(QWidget *parent = 0);
    ~easyInput();

protected:
    //鼠标拖动事件
    void mouseMoveEvent(QMouseEvent *);
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *);
    //鼠标松开事件
    void mouseReleaseEvent(QMouseEvent *);

    //事件过滤器,处理鼠标在汉字标签处单击操作
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::easyInput *ui;
    static easyInput *self;
    QWidget *fatherObject;

    //窗体宽度
    int frmWidth;
    //窗体高度
    int frmHeight;
    //桌面宽度
    int deskWidth;
    //桌面高度
    int deskHeight;

    //图标宽度
    int iconWidth;
    //图标高度
    int iconHeight;
    //间距
    int spacing;
    //顶部高度
    int topHeight;

    //数据库路径
    QString dbPath;
    //鼠标拖动自定义标题栏时的坐标
    QPoint mousePoint;
    //鼠标是否按下
    bool mousePressed;
    //当前大小写状态
    bool upper;

    //是否长按退格键
    bool isPress;
    //长按按钮
    QPushButton *btnPress;
    //退格键定时器
    QTimer *timerPress;

    //当前焦点的对象
    QWidget *currentWidget;
    //当前焦点的单行文本框
    QLineEdit *currentLineEdit;
    //当前焦点的多行文本框
    QTextEdit *currentTextEdit;
    //当前焦点的富文本框
    QPlainTextEdit *currentPlain;
    //当前焦点的文本浏览框
    QTextBrowser *currentBrowser;
    //当前焦点控件的类型
    QString currentEditType;
    //当前焦点控件属性
    QString currentFlag;

    //显示当前中英文状态
    QLabel *labStatus;
    //是否迷你模式
    bool mini;
    //是否数字键盘模式
    bool number;

    //当前输入法面板位置类型
    QString position;
    //当前输入法类型
    QString inputType;

    //汉字标签数组
    QList<QLabel *>labCh;
    //所有拼音链表
    QStringList allPY;
    //当前拼音链表
    QStringList currentPY;
    //当前拼音索引
    int currentPYIndex;
    //当前拼音数量
    int currentPYCount;

    //谷歌拼音内核
    GooglePinYin py;

private slots:
    //初始化属性
    void initProperty();
    //初始化窗体数据
    void initForm();
    //初始化数据库
    void initDb();

    //校验当前长按的按钮
    bool checkPress();
    //定时器处理退格键
    void reClicked();

    //输入法面板按键处理
    void btnClicked();

    //焦点改变事件槽函数处理
    void focusChanged(QWidget *oldWidget, QWidget *nowWidget);

    //查询汉字
    void selectChinese();
    //显示查询到的汉字
    void showChinese();
    //插入值到当前焦点控件
    void insertValue(const QString &value);
    //删除当前焦点控件的一个字符
    void deleteValue();

    //设置当前汉字
    void setChinese(int index);
    //清空当前汉字信息
    void clearChinese();    

public Q_SLOTS:
    //设置字库路径
    void setDbPath(const QString &dbPath);
    //设置字体名称及大小,包括按钮的字体大小和汉字部分的字体大小
    void setFontInfo(const QString &fontName, int btnFontSize, int labFontSize);

    //设置窗体尺寸
    void setFrmSize(int frmWidth, int frmHeight);
    //设置图标尺寸
    void setIconSize(int iconWidth, int iconHeight);

    //设置按钮之间的空隙间隔
    void setSpacing(int spacing);
    //设置顶部汉字区域高度
    void setTopHeight(int topHeight);

    //设置布局
    void setPlacement();

    //设置输入法弹出位置
    void setPosition(const QString &position);

    //设置输入法颜色样式
    void setMainStyle();
    //改变输入法面板样式
    void setColor(const QString &mainBackgroundColor, const QString &mainTextColor,
                  const QString &btnBackgroundColor, const QString &btnHoveColor,
                  const QString &btnHoveTextColor, const QString &labHoveColor,
                  const QString &labHoveTextColor);

    //设置当前输入法类型
    void setInputType(const QString &inputType);
    //改变字母大小写
    void setUpper(bool isUpper);
    //按钮改变成特殊字符
    void showChar(bool isChar);

    //显示输入法面板
    void showPanel();
    //隐藏输入法面板
    void hidePanel();
};

#endif // EASYINPUT_H

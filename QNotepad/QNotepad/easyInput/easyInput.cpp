#pragma execution_character_set("utf-8")

#include "easyinput.h"
#ifndef __arm__
#include "ui_easyinput.h"
#else
#include "$$../ui_easyInput.h"
#endif

easyInput *easyInput::self = NULL;
easyInput *easyInput::Instance(QWidget *parent)
{
    if (!self) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (!self) {
            self = new easyInput(parent);
        }
    }
    return self;
}

easyInput::easyInput(QWidget *parent) : QWidget(parent), ui(new Ui::easyInput), fatherObject(parent)
{
    ui->setupUi(this);
    this->initProperty();
    this->initForm();
    QTimer::singleShot(1000, this, SLOT(initDb()));
}

easyInput::~easyInput()
{
    timerPress->stop();
    delete ui;
}

void easyInput::mouseMoveEvent(QMouseEvent *e)
{
}

void easyInput::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void easyInput::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

//事件过滤器,用于识别鼠标单击汉字标签处获取对应汉字
bool easyInput::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            if (obj == ui->labCh1) {
                setChinese(0);
            } else if (obj == ui->labCh2) {
                setChinese(1);
            } else if (obj == ui->labCh3) {
                setChinese(2);
            } else if (obj == ui->labCh4) {
                setChinese(3);
            } else if (obj == ui->labCh5) {
                setChinese(4);
            } else if (currentEditType != "" && obj != ui->btnClose) {
                QString objName = obj->objectName();
                if (obj->parent() != 0x0 && !obj->property("noinput").toBool() && objName != "frmMainWindow"
                        && objName != "frmInputWindow" && objName != "qt_edit_menu" && objName != "labPY") {
                    if (obj->inherits("QGroupBox") || obj->inherits("QFrame") || obj->inherits("QMenu")) {
                        hidePanel();
                    } else {
                        showPanel();
                    }
                }
            }
            btnPress = (QPushButton *)obj;
            if (checkPress()) {
                isPress = true;
                timerPress->start(500);
            }
            return false;
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        btnPress = (QPushButton *)obj;
        if (checkPress()) {
            isPress = false;
            timerPress->stop();
        }

        return false;
    } else if (event->type() == QEvent::KeyPress) {
        //如果输入法窗体不可见,则不需要处理
        if (!isVisible() && !mini) {
            return QWidget::eventFilter(obj, event);
        }

        QString labText = ui->labPY->text();
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        //Shift切换输入法模式,esc键关闭输入法面板,空格取第一个汉字,退格键删除
        //中文模式下回车键取拼音,中文模式下当没有拼音时可以输入空格
        if (keyEvent->key() == Qt::Key_Space) {
            if (labText != "") {
                ui->labPY->setText("");
                setChinese(0);
                return true;
            } else {
                return false;
            }
        } else if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            if (labText != "") {
                insertValue(labText);
                ui->labPY->setText("");
                selectChinese();
            } else {
                //hidePanel(); //2019年12月4日 14:56:03
            }
            //return true;
        } else if (keyEvent->key() == Qt::Key_Control) {
            //
        } else if (keyEvent->key() == Qt::Key_Shift) {
                ui->btnType->click();
                return true;
        } else if (keyEvent->key() == Qt::Key_Escape) {
            ui->btnClose->click();
            return true;
        } else if (keyEvent->key() == Qt::Key_Backspace) {
            if (inputType == "chinese") {
                QString txt = labText;
                int len = txt.length();
                if (len > 0) {
                    ui->labPY->setText(txt.left(len - 1));
                    selectChinese();
                    return true;
                }
            }
        } else if (keyEvent->key() == Qt::Key_CapsLock) {
            ui->btnUpper->click();
            return true;
        } else if (keyEvent->text() == "+" || keyEvent->text() == "=") {
            if (labText != "") {
                ui->btnNext->click();
                return true;
            } else {
                return false;
            }
        } else if (keyEvent->text() == "-" || keyEvent->text() == "_") {
            if (labText != "") {
                ui->btnPre->click();
                return true;
            } else {
                return false;
            }
        } else {
            if (currentEditType == "QWidget") {
                return false;
            }

            QString key;
            if (inputType == "chinese") {
                key = keyEvent->text();
            } else if (inputType == "english") {
                if (upper) {
                    key = keyEvent->text().toUpper();
                } else {
                    key = keyEvent->text().toLower();
                }
            }

            if (!key.isEmpty()) {
                QList<QPushButton *> btn = ui->widgetMain->findChildren<QPushButton *>();
                foreach (QPushButton *b, btn) {
                    QString text = b->text();
                    if (!text.isEmpty() && text == key) {
                        b->click();
                        return true;
                    }
                }
            }
        }

        return false;
    }

    return QWidget::eventFilter(obj, event);
}

void easyInput::initProperty()
{
    /*设置控件的属性，方便判断 */
    ui->btn0->setProperty("btnNum", true);
    ui->btn1->setProperty("btnNum", true);
    ui->btn2->setProperty("btnNum", true);
    ui->btn3->setProperty("btnNum", true);
    ui->btn4->setProperty("btnNum", true);
    ui->btn5->setProperty("btnNum", true);
    ui->btn6->setProperty("btnNum", true);
    ui->btn7->setProperty("btnNum", true);
    ui->btn8->setProperty("btnNum", true);
    ui->btn9->setProperty("btnNum", true);

    ui->btnDot->setProperty("btnOther", true);
    ui->btnSpace->setProperty("btnOther", true);
    ui->btnEnter->setProperty("btnOther", true);
    ui->btnDelete->setProperty("btnOther", true);
    ui->btnUpper->setProperty("btnOther", true);
    ui->btnType->setProperty("btnOther", true);

    //将字母设置为btnLetter属性
    QList<QPushButton *> btns = ui->widgetMain->findChildren<QPushButton *>();
    foreach (QPushButton *btn, btns) {
        if(!btn->property("btnOther").toBool()||btn->property("btnNum").toBool())
        btn->setProperty("btnLetter", true);
    }

    //中文可选词组Label
    labCh.append(ui->labCh1);
    labCh.append(ui->labCh2);
    labCh.append(ui->labCh3);
    labCh.append(ui->labCh4);
    labCh.append(ui->labCh5);
}

void easyInput::initForm()
{
    deskWidth = fatherObject->geometry().width();
    deskHeight = fatherObject->geometry().height();

    dbPath = qApp->applicationDirPath();
    mousePoint = QPoint();
    mousePressed = false;
    upper = false;

    isPress = false;
    timerPress = new QTimer(this);
    connect(timerPress, SIGNAL(timeout()), this, SLOT(reClicked()));

    currentWidget = 0;
    currentLineEdit = 0;
    currentTextEdit = 0;
    currentPlain = 0;
    currentBrowser = 0;
    currentEditType = "";

    //输入法面板的字体名称和按钮字体大小即汉字区域字体大小
    setFontInfo(this->font().family(), 11, 10);

    //输入法面板的固定大小，输入法面板最小的宽度为480,也就是正点原子RGB最小分辨率的屏
    setFrmSize(480, 200);
    //图标固定大小
    setIconSize(20, 20);
    //按钮之间的间隔
    setSpacing(6);
    //顶部汉字区域高度
    setTopHeight(40);

    //设置按钮位置
    setPlacement();

    //输入法面板的显示位置
    setPosition("bottom");

    //输入法面板的样式
    setMainStyle();

    //输入法模式 english--英文模式  chinese--中文模式  char--特殊字符模式
    setInputType("chinese");

    //设置默认普通模式
    number = true;

    //绑定按钮到事件，方法是查找儿子，绑定信号槽
    QList<QPushButton *> btns = ui->widgetMain->findChildren<QPushButton *>();
    foreach (QPushButton *btn, btns) {
        connect(btn, SIGNAL(clicked()), this, SLOT(btnClicked()));
    }
    //关闭按钮不在widgetMain里面，所以只能别自绑定
    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(btnClicked()));

    //绑定全局改变焦点信号槽
    connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)), this, SLOT(focusChanged(QWidget *, QWidget *)));//2019年12月10日 14:59:37

    //绑定按键事件过滤器
    qApp->installEventFilter(this);
}

void easyInput::initDb()
{
    py.open(dbPath);
}

bool easyInput::checkPress()
{
    //只有属于输入法键盘的合法按钮才继续处理
    bool num_ok = btnPress->property("btnNum").toBool();
    bool other_ok = btnPress->property("btnOther").toBool();
    bool letter_ok = btnPress->property("btnLetter").toBool();

    if (num_ok || other_ok || letter_ok) {
        return true;
    }

    return false;
}

void easyInput::reClicked()
{
    if (isPress) {
        timerPress->setInterval(30);
        btnPress->click();
    }
}

void easyInput::btnClicked()
{
    //如果当前焦点控件类型为空,则返回不需要继续处理
    if (currentEditType == "") {
        return;
    }
    QString labText = ui->labPY->text();
    QPushButton *btn = (QPushButton *)sender();
    QString objectName = btn->objectName();

    if (objectName == "btnType") {
        if (inputType == "english") {
            setInputType("chinese");
        } else if (inputType == "chinese") {
            setInputType("char");
        } else if (inputType == "char") {
            setInputType("english");
        }

        upper = false;
        setUpper(upper);
    } else if (objectName == "btnUpper") {
        //大小写模式都要切换到英文状态
        setInputType("english");

        upper = !upper;
        setUpper(upper);
    } else if (objectName == "btnDelete") {
        //如果当前是中文模式,则删除对应拼音,删除完拼音之后再删除对应文本输入框的内容
        if (inputType == "chinese") {
            QString txt = labText;
            int len = txt.length();

            if (len > 0) {
                ui->labPY->setText(txt.left(len - 1));
                selectChinese();
            } else {
                deleteValue();
            }
        } else {
            deleteValue();
        }
    } else if (objectName == "btnPre") {
        if (currentPYIndex >= 10) {
            //每次最多显示 5 个汉字,所以每次向前的时候索引要减 5 * 2
            if (currentPYIndex % 5 == 0) {
                currentPYIndex -= 10;
            } else {
                currentPYIndex = currentPYCount - (currentPYCount % 5) - 5;
            }
        } else {
            currentPYIndex = 0;
        }

        showChinese();
    } else if (objectName == "btnNext") {
        if (currentPYIndex < currentPYCount - 1) {
            showChinese();
        }
    } else if (objectName == "btnClose") {
        hidePanel();
        clearChinese();
        ui->labPY->setText("");
    } else if (objectName == "btnSpace") {
        //如果中文模式而且有待输入字母,判断是否有中文则插入第一个中文否则插入字母
        if (inputType == "chinese" && labText != "") {
            if (ui->labCh1->text().isEmpty()) {
                insertValue(labText);
            } else {
                setChinese(0);
            }
        } else {
            insertValue(" ");
        }
    } else if (objectName == "btnEnter") {
        //如果中文模式而且有待输入字母则立即插入字母
        if (inputType == "chinese" && labText != "") {
            insertValue(labText);
        }

        if (currentLineEdit != 0) {
            hidePanel();
            QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier, QString("\n"));
            QApplication::sendEvent(currentLineEdit, &keyPress);
        } else {
            insertValue("\n");
        }
    } else {
        QString value = btn->text();

        //如果是&按钮，因为对应&被过滤,所以真实的text为去除前面一个&字符
        if (value == "&&") {
            value = "&";
        }

        //当前不是中文模式,则单击按钮对应text为传递参数
        if (inputType != "chinese") {
            insertValue(value);
        } else {
            //中文模式下,不允许输入特殊字符,单击对应数字按键取得当前索引的汉字
            if (btn->property("btnOther").toBool()) {
                if (labText.length() == 0) {
                    insertValue(value);
                }
            } else if (btn->property("btnNum").toBool()) {
                if (labText.length() == 0) {
                    insertValue(value);
                } else if (objectName == "btn1") {
                    setChinese(0);
                } else if (objectName == "btn2") {
                    setChinese(1);
                } else if (objectName == "btn3") {
                    setChinese(2);
                } else if (objectName == "btn4") {
                    setChinese(3);
                } else if (objectName == "btn5") {
                    setChinese(4);
                }
            } else if (btn->property("btnLetter").toBool()) {
                ui->labPY->setText(labText + value);
                selectChinese();
            }
        }
    }
}

void easyInput::focusChanged(QWidget *oldWidget, QWidget *nowWidget)
{
    //qDebug() << "oldWidget:" << oldWidget << "nowWidget:" << nowWidget;
    if (nowWidget != 0 && !this->isAncestorOf(nowWidget)) {
        //在Qt5和linux系统中(嵌入式linux除外),当输入法面板关闭时,焦点会变成无,然后焦点会再次移到焦点控件处
        //这样导致输入法面板的关闭按钮不起作用,关闭后马上有控件获取焦点又显示.
        //为此,增加判断,当焦点是从有对象转为无对象再转为有对象时不要显示.
        //这里又要多一个判断,万一首个窗体的第一个焦点就是落在可输入的对象中,则要过滤掉
        static bool isFirst = true;
#ifndef __arm__
        if (oldWidget == 0x0 && !isFirst) {
            QTimer::singleShot(0, this, SLOT(hidePanel()));
            return;
        }
#endif

        //如果对应属性noinput为真则不显示
        if (nowWidget->property("noinput").toBool()) {
            QTimer::singleShot(0, this, SLOT(hidePanel()));
            return;
        }

        isFirst = false;
        currentFlag = nowWidget->property("flag").toString();

        if (nowWidget->inherits("QLineEdit")) {
            currentLineEdit = (QLineEdit *)nowWidget;
            if (!currentLineEdit->isReadOnly()) {
                currentEditType = "QLineEdit";
                showPanel();
            } else {
                currentEditType = "";
                hidePanel();
            }
        } else if (nowWidget->inherits("QTextEdit")) {
            currentTextEdit = (QTextEdit *)nowWidget;
            if (!currentTextEdit->isReadOnly()) {
                currentEditType = "QTextEdit";
                showPanel();
            } else {
                currentEditType = "";
                hidePanel();
            }
        } else {
            //需要将输入法切换到最初的原始状态--小写,同时将之前的对象指针置为零
            currentWidget = 0;
            currentLineEdit = 0;
            currentTextEdit = 0;
            currentPlain = 0;
            currentBrowser = 0;
            currentEditType = "";
            //setInputType("english");
            hidePanel();
        }

        //根据用户选择的输入法位置设置-居中显示-底部填充-显示在输入框正下方
        int height = this->height();
//        if(fatherObject->geometry().height()<480)
//            height=150;
        //嵌入式设备，只设置在底部
        this->setGeometry(0, deskHeight - height, deskWidth, height);
    }

    //设置按钮的大小及位置
    ui->btnClose->setMaximumWidth(ui->btn0->width());
    ui->btnClose->setMinimumWidth(ui->btn0->width());
    ui->btnPre->setMaximumWidth(ui->btn0->width());
    ui->btnPre->setMinimumWidth(ui->btn0->width());
    ui->btnNext->setMaximumWidth(ui->btn0->width());
    ui->btnNext->setMinimumWidth(ui->btn0->width());

    int fitWidth = (ui->widgetTop->width()-ui->btn0->width()*3-ui->widgetMain->layout()->spacing()*6.7)/6;
    ui->labCh1->setMaximumWidth(fitWidth);
    ui->labCh1->setMinimumWidth(fitWidth);
    ui->labCh2->setMaximumWidth(fitWidth);
    ui->labCh2->setMinimumWidth(fitWidth);
    ui->labCh3->setMaximumWidth(fitWidth);
    ui->labCh3->setMinimumWidth(fitWidth);
    ui->labCh4->setMaximumWidth(fitWidth);
    ui->labCh4->setMinimumWidth(fitWidth);
    ui->labCh5->setMaximumWidth(fitWidth);
    ui->labCh5->setMinimumWidth(fitWidth);
    ui->labPY->setMaximumWidth(fitWidth);
    ui->labPY->setMinimumWidth(fitWidth);
}

void easyInput::selectChinese()
{
    //清空汉字
    clearChinese();

    QString currentPY = ui->labPY->text();
    int count = py.select(currentPY);
    QStringList list;
    for (int i = 0; i < count; i++) {
        QString str = py.getChinese(i);
        list << str;
    }

    foreach (QString txt, list) {
        if (txt.length() > 0) {
            allPY.append(txt);
            currentPYCount++;
        }
    }

    //显示汉字
    showChinese();
}

void easyInput::showChinese()
{
    //每个版面最多显示 chineseCount 个汉字
    int count = 0;
    currentPY.clear();

    for (int i = 0; i < labCh.count(); i++) {
        labCh.at(i)->setText("");
    }

    for (int i = currentPYIndex; i < currentPYCount; i++) {
        if (count == 5) {
            break;
        }

        QString txt = QString("%1. %2").arg(count + 1).arg(allPY.at(currentPYIndex));
        currentPY.append(allPY.at(currentPYIndex));
        labCh.at(count)->setText(txt);
        count++;
        currentPYIndex++;
    }
    //qDebug() << "currentPYIndex:" << currentPYIndex << "currentPYCount:" << currentPYCount;
}

void easyInput::insertValue(const QString &v)
{
    QString value = v;
    if (currentEditType == "QLineEdit") {
        currentLineEdit->insert(currentLineEdit->property("upper").toBool() ? value.toUpper() : value);
    } else if (currentEditType == "QTextEdit") {
        currentTextEdit->insertPlainText(currentTextEdit->property("upper").toBool() ? value.toUpper() : value);
    }else if (currentEditType == "QWidget") {
        QStringList list = value.split("");
        foreach (QString str, list) {
            if (!str.isEmpty()) {
                QKeyEvent keyPress(QEvent::KeyPress, 0, Qt::NoModifier, QString(str));
                QApplication::sendEvent(currentWidget, &keyPress);
            }
        }
    }

    //将所有汉字标签可见
    ui->labCh3->setVisible(true);
    ui->labCh4->setVisible(true);
    ui->labCh5->setVisible(true);
    ui->labPY->setText("");
}

void easyInput::deleteValue()
{
    if (currentEditType == "QLineEdit") {
        currentLineEdit->backspace();
    } else if (currentEditType == "QTextEdit") {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier, QString());
        QApplication::sendEvent(currentTextEdit, &keyPress);
    }
}

void easyInput::setChinese(int index)
{
    int count = currentPY.count();
    if (count > index) {
        insertValue(currentPY.at(index));
        //添加完一个汉字后,清空当前汉字信息,等待重新输入
        clearChinese();
        ui->labPY->setText("");
    }
}

void easyInput::clearChinese()
{
    //清空汉字,重置索引
    for (int i = 0; i < labCh.count(); i++) {
        labCh.at(i)->setText("");
    }

    allPY.clear();
    currentPY.clear();
    currentPYIndex = 0;
    currentPYCount = 0;
}

void easyInput::setDbPath(const QString &dbPath)
{
    this->dbPath = dbPath;
}

void easyInput::setFontInfo(const QString &fontName, int btnFontSize, int labFontSize)
{
    QFont btnFont(fontName, btnFontSize);
    QFont labFont(fontName, labFontSize);

    QList<QPushButton *> btns = this->findChildren<QPushButton *>();
    foreach (QPushButton *btn, btns) {
        btn->setFont(btnFont);
    }

    QList<QLabel *> labs = ui->widgetTop->findChildren<QLabel *>();
    foreach (QLabel *lab, labs) {
        lab->setFont(labFont);
    }

    ui->btnPre->setFont(btnFont);
    ui->btnNext->setFont(btnFont);
}

void easyInput::setFrmSize(int frmWidth, int frmHeight)
{
    this->frmWidth = frmWidth;
    this->frmHeight = frmHeight;
    this->setMinimumSize(frmWidth, frmHeight);
}

void easyInput::setIconSize(int iconWidth, int iconHeight)
{
    this->iconWidth = iconWidth;
    this->iconHeight = iconHeight;
    QSize icoSize(iconWidth, iconHeight);
    QSize icoSize2(iconWidth - 5, iconHeight - 5);
    QSize icoSize3(iconWidth + 3, iconHeight + 3);

    ui->btnSpace->setIconSize(icoSize);
    ui->btnClose->setIconSize(icoSize);
    ui->btnType->setIconSize(icoSize);
    ui->btnEnter->setIconSize(icoSize2);
    ui->btnUpper->setIconSize(icoSize2);
    ui->btnDelete->setIconSize(icoSize3);
}

void easyInput::setSpacing(int spacing)
{
    this->spacing = spacing;
    ui->widgetMain->layout()->setSpacing(spacing);
    ui->widgetPreNext->layout()->setSpacing(spacing);
}

void easyInput::setTopHeight(int topHeight)
{
    this->topHeight = topHeight;
    ui->widgetTop->setMaximumHeight(ui->btnq->geometry().height());//设置中文选择栏的输入法最大高度
    ui->widgetTop->setMinimumHeight(ui->btnq->geometry().height());//设置中文选择栏的输入法最小高度

}

void easyInput::setPlacement()
{
    ui->widgetMain->setVisible(true);
    ui->widgetPreNext->setVisible(true);
    ui->widgetTop->layout()->setContentsMargins(9, 4, 9, 0);
}

void easyInput::setPosition(const QString &position)
{
    this->position = position;
}

void easyInput::setMainStyle()
{
    QString iconType = "white";
    setColor("#191919", "#F3F3F3", "#313131", "#4189D3", "#F3F3F3", "#F95717", "#F3F3F3");
    QString icon_upper = QString(":/image/upper_%1.png").arg(iconType);
    QString icon_delete = QString(":/image/delete_%1.png").arg(iconType);
    ui->btnUpper->setIcon(QIcon(icon_upper));
    ui->btnDelete->setIcon(QIcon(icon_delete));
}

void easyInput::setColor(const QString &mainBackgroundColor, const QString &mainTextColor, const QString &btnBackgroundColor,
                         const QString &btnHoveColor, const QString &btnHoveTextColor, const QString &labHoveColor, const QString &labHoveTextColor)
{
    QStringList qss;
    qss.append(QString("QWidget#widgetPreNext{background:none;}"));
    qss.append(QString("QWidget#widgetMain,QWidget#widgetTop,QWidget#widgetNumber{background-color:%1;}").arg(mainBackgroundColor));
    qss.append(QString("QPushButton{border-width:0px;border-radius:3px;color:%1;}").arg(mainTextColor));
    qss.append(QString("QPushButton{padding:0px;background-color:%1;}").arg(btnBackgroundColor));
    qss.append(QString("QPushButton:pressed,QPushButton:hover{background-color:%1;color:%2;}").arg(btnHoveColor).arg(btnHoveTextColor));
    qss.append(QString("QLabel{border-width:0px;border-radius:3px;color:%1;background:none;}").arg(mainTextColor));
    qss.append(QString("QLabel:pressed,QLabel:hover{background-color:%1;color:%2;}").arg(labHoveColor).arg(labHoveTextColor));
    //设置输入法的主样式
    this->setStyleSheet(qss.join("QWidget{font-size:12px;}"));
}

void easyInput::setInputType(const QString &inputType)
{
    this->inputType = inputType;
    if (inputType == "english") {
        ui->btnType->setText("En");
        //ui->widgetTop->setEnabled(false);
        showChar(false);
    } else if (inputType == "chinese") {
        ui->btnType->setText("中");
        ui->widgetTop->setEnabled(true);
        showChar(false);
    } else if (inputType == "char") {
        ui->btnType->setText("符");
        ui->widgetTop->setEnabled(false);
        showChar(true);
    }

    //每次切换到模式,都要执行清空之前中文模式下的信息
    clearChinese();
    ui->labPY->setText("");
}

void easyInput::setUpper(bool isUpper)
{
    QList<QPushButton *> btn = ui->widgetMain->findChildren<QPushButton *>();
    foreach (QPushButton *b, btn) {
        if (b->property("btnLetter").toBool()) {
            if (isUpper) {
                b->setText(b->text().toUpper());
            } else {
                b->setText(b->text().toLower());
            }
        }
    }
}

void easyInput::showChar(bool isChar)
{
    static QStringList charList1;
    charList1<<"!"<<"@"<<"#"<<"$"<<"%"<<"^"<<"&&"<<"?"<<"["<<")"<<"~"<<"`"<<"-"<<"("<<"+"<<"="<<"{"<<"}"<<"_"
            <<"]"<<"|"<<"\\"<<":"<<";"<<"\""<<"<"<<">"<<"'"<<"*"<<","<<"/"<<"："<<"，"<<"。"<<"？"<<"；";
    static QStringList charList2;
    charList2<<"3"<<"5"<<"6"<<"7"<<"8"<<"9"<<"s"<<"d"<<"g"<<"k"<<"l"<<"z"<<"x"<<"j"<<"c"<<"v"<<"b"<<"n"<<"m"
            <<"h"<<"0"<<"4"<<"1"<<"2"<<"f"<<"o"<<"p"<<"a"<<"i"<<"q"<<"u"<<"w"<<"e"<<"y"<<"t"<<"r";
    if (isChar) {
        QList<QPushButton *> btns = ui->widgetMain->findChildren<QPushButton *>();
        int i=0;
        foreach (QPushButton *btn, btns) {
            if(!btn->property("btnOther").toBool()){
                btn->setText(charList1[i]);
                i++;
            }
        }
    } else {
        QList<QPushButton *> btns = ui->widgetMain->findChildren<QPushButton *>();
        int i=0;
        foreach (QPushButton *btn, btns) {
            if(!btn->property("btnOther").toBool()){
                btn->setText(charList2[i]);
                i++;
            }
        }
    }
}

void easyInput::showPanel()
{
    if (!isEnabled()) {
        return;
    }
    this->setVisible(true);
    int width = ui->btna->width();
    width = width > 35 ? width : 35;
    ui->btnPre->setFixedWidth(width);
    ui->btnNext->setFixedWidth(width);
}

void easyInput::hidePanel()
{
    this->setVisible(false);
}


#ifndef CONFIG_H
#define CONFIG_H

#define FILEVIEWLISTWIDGET_STYLE                "QListWidget::item:selected:active { background:rgba(130, 130, 130, 50%);}"\
                                                "QListWidget::item:hover:active { background: rgba(144, 144, 150, 50%);}"\
                                                "QListWidget {background-color: rgba(255, 255, 255, 5%);outline:none;border-image:transparent;color:white}"

#define FILEVIEWLISTWIDGET_VERTICAL_STYLE       "QScrollBar:vertical {width:8px; background:rgba(0,0,0,0%);}"\
                                                "QScrollBar::handle:vertical {width:8px; background:rgba(200, 200, 200,25%); border-radius:4px;}"\
                                                "QScrollBar::add-line:vertical {width:0px; height:0px;}"\
                                                "QScrollBar::sub-line:vertical {width:0px; height:0px;}"\
                                                "QScrollBar::handle:vertical:hover {width:8px; background:rgba(200, 200, 200,80%); border-radius:4px;}"\
                                                "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical {background:rgba(0,0,0,0%);}"

#define PATHLISTWIDGET_STYLE                    "QListWidget::item:selected:active { background: rgba(130, 130, 130,50%);}"\
                                                "QListWidget::item:hover:active { background: rgba(144, 144, 150,50%);}"\
                                                "QListWidget {color: white; outline: none;border-image:transparent;background-color: transparent; }"

#define UPPUSHBUTTON_STYLE_1                    "QPushButton {border-image:url(:icon/Resource/icon/up_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/up_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/up_1.png);}"

#define UPPUSHBUTTON_STYLE_2                    "QPushButton {border-image:url(:icon/Resource/icon/up_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/up_1.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/up_1.png);}"

#define CURRTENPLACELINEEDIT_STYLE              "QLineEdit {color: white;background:transparent;border-color:#999999;border-width:1;border-radius:5px;"\
                                                "border-style:solid }"

#endif // CONFIG_H


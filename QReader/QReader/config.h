#ifndef CONFIG_H
#define CONFIG_H

#define TEXTBROWSER_STYLE               "QScrollBar:vertical {width:8px; background:rgba(0,0,0,0%);}"\
                                        "QScrollBar::handle:vertical {width:8px; background:rgba(78, 76, 69,50%); border-radius:4px;}"\
                                        "QScrollBar::add-line:vertical {width:0px; height:0px;}"\
                                        "QScrollBar::sub-line:vertical {width:0px; height:0px;}"\
                                        "QScrollBar::handle:vertical:hover {width:8px; background:rgba(78, 76, 69,100%); border-radius:4px;}"\
                                        "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical {background:rgba(0,0,0,0%);}"
#define MENU_STYLE                      "\
                                        QMenu {\
                                        background-color:rgb(89,87,87); \
                                        border: 3px solid rgb(222,222,255);\
                                        }\
                                        QMenu::item {\
                                        font-size: 10pt; \
                                        color: rgb(225,225,225);\
                                        border: 3px solid rgb(60,60,60);\
                                        background-color:rgb(89,87,87);\
                                        padding:4px 4px; \
                                        margin:2px 2px;\
                                         }\
                                        QMenu::item:selected { \
                                        background-color:rgb(180,180,255);\
                                        }\
                                        QMenu::item:pressed {\
                                        border: 1px solid rgb(60,60,61); \
                                        background-color: rgb(220,80,6); \
                                        }"
#endif // CONFIG_H


#ifndef CONFIG_H
#define CONFIG_H

#define APP_CONFIG_AUDIO_FILE_PATH "./Resource/audio"
#define USER_VOLUME 50

#define RECORDERBUTTON_STYLE_1      "QPushButton {border-image:url(:/icon/Resource/icons/recorder_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/recorder_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/recorder_1);}"

#define RECORDERBUTTON_STYLE_2      "QPushButton {border-image:url(:/icon/Resource/icons/recorder_stop_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/recorder_stop_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/recorder_stop_1);}"

#define RECORDERBUTTON_STYLE        "QPushButton {border-image:url(:/icon/Resource/icons/recorder_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/recorder_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/recorder_1);}"

#define RECORDERLISTWIDGET_STYLE    "QListWidget{background-color: rgba(255, 255, 255, 10%);outline:none;border-image:transparent;}"\
                                    "QListWidget::item:selected:active{background: rgb(0, 106, 181);}"\
                                    "QListWidget::item:hover:active{background: qlineargradient(spread:repeat, x2:0, y2:1, x2:1, y2:0,"\
                                    "stop:0 #1d2437, stop:0.25 #1d2437, stop:0.5 #ff5b53, stop:1 #ff5b53);}"

#define PREVIOUSPUSHBUTTON_STYLE    "QPushButton {border-image:url(:/icon/Resource/icons/btn_previous_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/btn_previous_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/btn_previous_1);}"

#define NEXTPUSHBUTTON_STYLE        "QPushButton {border-image:url(:/icon/Resource/icons/btn_next_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/btn_next_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/btn_next_1);}"

#define PLAYORPAUSEPUSHBUTTON_STYLE_1 "QPushButton {border-image:url(:/icon/Resource/icons/btn_play_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/btn_play_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/btn_pause_1);}"

#define PLAYORPAUSEPUSHBUTTON_STYLE_2 "QPushButton {border-image:url(:/icon/Resource/icons/btn_pause_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/btn_pause_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/btn_pause_1);}"

#define MEDIASLIDER_STYLE           "QSlider{\
                                    border-color: #bcbcbc;background-color:transparent;\
                                    }\
                                    QSlider::groove:horizontal {\
                                    border: 1px solid #999999;\
                                    height: 1px;\
                                    margin: 0px 0; \
                                    left: 5px; right: 5px;\
                                    }\
                                    QSlider::handle:horizontal {\
                                    border: 0px ;\
                                    border-image:  url(:icon/Resource/icons/handle.png);\
                                    width: 20px; \
                                    margin: -10px -7px -10px -7px; \
                                    } \
                                    QSlider::add-page:horizontal{\
                                    background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #bcbcbc, stop:0.25 #bcbcbc, stop:0.5 #bcbcbc, stop:1 #bcbcbc); \
                                    }\
                                    QSlider::sub-page:horizontal{ \
                                    background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #f73d31, stop:0.25 #f73d31, stop:0.5 #f73d31, stop:1 #439cf3); \
                                    }"

#define MUTEPUSHBUTTON_STYLE_1      "QPushButton {border-image:url(:/icon/Resource/icons/btn_vol_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/btn_vol_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/btn_vol_1);}"

#define MUTEPUSHBUTTON_STYLE_2       "QPushButton {border-image:url(:/icon/Resource/icons/btn_mute_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/btn_mute_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/btn_mute_1);}"

#define DELETEPUSHBUTTON_STYLE      "QPushButton {border-image:url(:/icon/Resource/icons/delete_1);}"\
                                    "QPushButton:hover {border-image:url(:/icon/Resource/icons/delete_2);}"\
                                    "QPushButton:pressed {border-image:url(:/icon/Resource/icons/delete_1);}"

#define VOLUMESLIDER_STYLE      "QSlider{\
                                border-color: #cbcbcb;background-color:transparent;\
                                }\
                                QSlider::groove:vertical {\
                                background: #cbcbcb;\
                                width: 6px;\
                                border-radius: 1px;\
                                padding-left:-1px;\
                                padding-right:-1px;\
                                padding-top:-1px;\
                                padding-bottom:-1px;\
                                } \
                                QSlider::sub-page:vertical {\
                                background: #cbcbcb;\
                                border-radius: 2px;\
                                } \
                                QSlider::add-page:vertical {\
                                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\
                                    stop:0 #439cf4, stop:1 #439cf4);\
                                background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1, \
                                    stop: 0 #f73d31, stop: 1 #f73d31);\
                                width: 10px;\
                                border-radius: 2px; \
                                }\
                                QSlider::handle:vertical\
                                {   \
                                    border-image: url(:icon/Resource/icons/handle.png);\
                                     margin: -2px -15px -2px -15px;\
                                    height: 35px;\
                                }"
/* 窗口话询问按钮确认键样式表 */
#define BTN_YES_STYLE               "background-color:#89AFDE;"\
                                    "border-style: outset;"\
                                    "border-width: 2px;"\
                                    "border-radius: 10px;"\
                                    "border-color: beige;"\
                                    "font: bold 15px;"\
                                    "min-width: 3em;"\
                                    "min-height: 1em;"
/* 窗口话询问按钮取消键样式表 */
#define BTN_CANCEL_STYLE            "background-color:#89AFDE;"\
                                    "border-style: outset;"\
                                    "border-width: 2px;"\
                                    "border-radius: 10px;"\
                                    "border-color: beige;"\
                                    "font: bold 15px;"\
                                    "min-width: 3em;"\
                                    "min-height: 1em;"
#endif // CONFIG_H


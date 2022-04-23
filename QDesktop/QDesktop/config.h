#ifndef CONFIG_H
#define CONFIG_H
/* 歌曲目录  */
#define APP_CONFIG_AUDIO_FILE_PATH               "./Resource/audio"
#define SPEAKERS_STYLE                          "QSlider{background-color:rgba(200,200,200,50%);\
                                                border-color: #bcbcbc;\
                                                }\
                                                QSlider::groove:horizontal {\
                                                background:transparent;height:1px;border-radius: 1px;left:3px;right:3px}\
                                                QSlider::handle:horizontal {\
                                                border: 0px ;\
                                                border-image:  url(:icon/Resource/icon/handle2.png);\
                                                width: 30px; \
                                                margin: -15px -8px -15px -8px; \
                                                } \
                                                QSlider::add-page:horizontal{\
                                                background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #bcbcbc, stop:0.25 #bcbcbc, stop:0.5 #bcbcbc, stop:1 #bcbcbc); \
                                                }\
                                                QSlider::sub-page:horizontal{ \
                                                background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #439cf3, stop:0.25 #439cf3, stop:0.5 #439cf3, stop:1 #439cf3); \
                                                }"\
                                                "QSlider::groove:hover:horizontal {background:transparent;height:2px}"

#define MUSICSLIDER_STYLE                       "QSlider{\
                                                border-color: #bcbcbc;\
                                                }\
                                                QSlider::groove:horizontal {\
                                                background:transparent;height:1px;border-radius: 1px;left:3px;right:3px}\
                                                QSlider::handle:horizontal {\
                                                border: 0px ;\
                                                border-image:  url(:icon/Resource/icon/handle.png);\
                                                width: 20px; \
                                                margin: -10px -7px -10px -7px; \
                                                } \
                                                QSlider::add-page:horizontal{\
                                                background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #bcbcbc, stop:0.25 #bcbcbc, stop:0.5 #bcbcbc, stop:1 #bcbcbc); \
                                                }\
                                                QSlider::sub-page:horizontal{ \
                                                background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #439cf3, stop:0.25 #439cf3, stop:0.5 #439cf3, stop:1 #439cf3); \
                                                }"\
                                                "QSlider::groove:hover:horizontal {background:transparent;height:2px}"


#endif // CONFIG_H


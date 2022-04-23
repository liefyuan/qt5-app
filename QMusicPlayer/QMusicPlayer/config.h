#ifndef CONFIG_H
#define CONFIG_H

//开机默认音量
#define APP_CONFIG_PLAYER_DEFAULT_VOLUME         100      // range: 0~100

//指定路径
//<包括：音频文件所在路径、歌手写真所在路径、歌词文件所在路径>

#define APP_CONFIG_AUDIO_FILE_PATH               "./Resource/audio"
#define APP_CONFIG_SINGER_PHOTO_FILE_PATH        "./Resource/image"
#define APP_CONFIG_LRC_FILE_PATH                 "./Resource/lrc"

//开机默认播放模式，默认是单曲播放
//只能选择以下5种模式之一
#define PLAYBACK_MODE_CURRENT_ITEM_ONCE    0               // 单曲播放
#define PLAYBACK_MODE_CURRENT_ITEM_IN_LOOP 1               // 单曲循环
#define PLAYBACK_MODE_SEQUENTIAL           2               // 顺序播放
#define PLAYBACK_MODE_LOOP                 3               // 列表循环
#define PLAYBACK_MODE_RANDOM               4               // 随机播放
#define APP_CONFIG_PLAYER_DEFAULT_PLAYBACK_MODE  PLAYBACK_MODE_CURRENT_ITEM_ONCE

//主窗口默认背景
#define APP_CONFIG_MAINWINDOW_BACKGROUND_IMAGE  "QMainWindow{border-image:url(Resource/background/QBack2.png)}"

//前一曲按钮样式
#define PREVIOUSPUSHBUTTON_STYLE                "QPushButton {border-image:url(:icon/Resource/icon/btn_previous_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_previous_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_previous_1.png);}"
//下一曲按钮样式
#define NEXTPUSHBUTTON_STYLE                    "QPushButton {border-image:url(:icon/Resource/icon/btn_next_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_next_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_next_1.png);}"

//播放或暂停按钮样式一
#define playOrPausePushButton_STYLE_1           "QPushButton {border-image:url(:icon/Resource/icon/btn_play_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_play_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_play_1.png);}"

//播放或暂停按钮样式二
#define playOrPausePushButton_STYLE_2           "QPushButton {border-image:url(:icon/Resource/icon/btn_pause_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_pause_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_pause_1.png);}"
//静音按钮样式1
#define MUTEPUSHBUTTON_STYLE_1                  "QPushButton {border-image:url(:icon/Resource/icon/btn_vol_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_vol_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_vol_1.png);}"
//静音按钮样式2
#define MUTEPUSHBUTTON_STYLE_2                  "QPushButton {border-image:url(:icon/Resource/icon/btn_mute_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_mute_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_mute_1.png);}"
//歌曲播放进度条样式
#define SONGSPLAYSLIDER_STYLE                   "QSlider{\
                                                border-color: #bcbcbc;\
                                                }\
                                                QSlider::groove:horizontal {\
                                                border: 1px solid #999999;\
                                                height: 1px;\
                                                margin: 0px 0; \
                                                left: 5px; right: 5px;\
                                                }\
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
                                                }"
//音量条样式
#define VOLUMESLIDER_STYLE                  "QSlider{\
                                                 border-color: #cbcbcb;\
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
                                                 stop: 0 #439cf4, stop: 1 #439cf4);\
                                             width: 10px;\
                                             border-radius: 2px; \
                                             }\
                                             QSlider::handle:vertical\
                                             {   \
                                                 border-image: url(:icon/Resource/icon/handle.png);\
                                                  margin: -2px -15px -2px -15px;\
                                                 height: 35px;\
                                             }"
//歌曲列表的样式
#define SONGSLIST_STYLE                         "QListWidget{background-color: rgba(255, 255, 255, 10%);outline:none}"\
                                                "QListWidget::item:selected:active{background: rgb(0, 106, 181);}"\
                                                "QListWidget::item:hover:active{background: qlineargradient(spread:repeat, x2:0, y2:1, x2:1, y2:0, stop:0 #a1a112, stop:0.25 #a1a112, stop:0.5 #f9f965, stop:1 #f9f965);}"

//设置列表的样式
#define SETTINGS_STYLE                          "QListWidget{background-color: rgba(255, 255, 255, 10%);outline:none;}"\
                                                "QListWidget::item:selected:active{background: rgb(0, 106, 181);}"\
                                                "QListWidget::item:hover:active{background: transparent;}"
//播放模式列表
#define PLAYBACKMODELIST_STYLE                  "QListWidget{background-color: rgba(25, 38, 58, 80%);outline:none}"\
                                                "QListWidget::item:selected:active{background: rgb(0, 106, 181);}"\
                                                "QListWidget::item:hover:active{background: qlineargradient(spread:repeat, x2:0, y2:1, x2:1, y2:0, stop:0 #a1a112, stop:0.25 #a1a112, stop:0.5 #f9f965, stop:1 #f9f965);}"
//播放模式样式0，单曲播放
#define PLAYBACKMODE_STYLE_0                    "QPushButton {border-image:url(:icon/Resource/icon/btn_listsingle_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_listsingle_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_listsingle_1.png);}"
//播放模式样式1，单曲循环
#define PLAYBACKMODE_STYLE_1                    "QPushButton {border-image:url(:icon/Resource/icon/btn_listscircle_single_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_listscircle_single_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_listscircle_single_1.png);}"
//播放模式样式2，顺序播放
#define PLAYBACKMODE_STYLE_2                    "QPushButton {border-image:url(:icon/Resource/icon/btn_listjump_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_listjump_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_listjump_1.png);}"
//播放模式样式3，列表循环
#define PLAYBACKMODE_STYLE_3                    "QPushButton {border-image:url(:icon/Resource/icon/btn_listcircle_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_listcircle_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_listcircle_1.png);}"
//播放模式样式4，随机播放
#define PLAYBACKMODE_STYLE_4                    "QPushButton {border-image:url(:icon/Resource/icon/btn_listrandom_1.png);}"\
                                                "QPushButton:hover{border-image:url(:icon/Resource/icon/btn_listrandom_2.png);}"\
                                                "QPushButton:pressed{border-image:url(:icon/Resource/icon/btn_listrandom_1.png);}"

#endif // CONFIG_H


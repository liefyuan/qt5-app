#ifndef CONFIG_H
#define CONFIG_H

#define AXIS_X_MIN   0
#define AXIS_X_MAX   50
#define AXIS_Y_MIN   0
#define AXIS_Y_MAX   100
#define COUNT_MAX    500000
#define  TEMP_INPUT_PATH                "cat /sys/class/hwmon/hwmon0/temp1_input"



#define PBRESET_STYLE                   "QPushButton {border-image: url(:/icon/Resource/icon/reset3.png); outline: 0px;}"\
                                        "QPushButton:hover {border-image: url(:/icon/Resource/icon/reset2.png)}"\
                                        "QPushButton:pressed {border-image: url(:/icon/Resource/icon/reset3.png)}"
#define PBSTOP_STYLE                    "QPushButton {border-image: url(:/icon/Resource/icon/stop3.png); outline: 0px;}"\
                                        "QPushButton:hover {border-image: url(:/icon/Resource/icon/stop2.png)}"\
                                        "QPushButton:pressed {border-image: url(:/icon/Resource/icon/stop3.png)}"
#define PBSTART_STYLE                   "QPushButton {border-image: url(:/icon/Resource/icon/start3.png); outline: 0px;}"\
                                        "QPushButton:hover {border-image: url(:/icon/Resource/icon/start2.png)}"\
                                        "QPushButton:pressed {border-image: url(:/icon/Resource/icon/start3.png)}"

#endif // CONFIG_H


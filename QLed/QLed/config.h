#ifndef CONFIG_H
#define CONFIG_H


/* led与蜂鸣器都属于led节点下，本次LED2实质是蜂鸣器 */
#define LED1_PATH                               "/sys/class/leds/sys-led"
#define LED2_PATH                               "/sys/class/leds/beep"

/* 窗口话询问按钮确认键样式表 */
#define BTN_YES_STYLE                           "background-color:#89AFDE;"\
                                                "border-style: outset;"\
                                                "border-width: 2px;"\
                                                "border-radius: 10px;"\
                                                "border-color: beige;"\
                                                "font: bold 15px;"\
                                                "min-width: 3em;"\
                                                "min-height: 1em;"
/* 窗口话询问按钮取消键样式表 */
#define BTN_CANCEL_STYLE                        "background-color:#89AFDE;"\
                                                "border-style: outset;"\
                                                "border-width: 2px;"\
                                                "border-radius: 10px;"\
                                                "border-color: beige;"\
                                                "font: bold 15px;"\
                                                "min-width: 3em;"\
                                                "min-height: 1em;"
#endif // CONFIG_H


#!/bin/sh
#设置捕获的音量
amixer cset name='Capture Volume' 63,63
#音频输入，左声道管理
#Turn on Left Input Mixer Boost
amixer sset 'Left Input Mixer Boost' off
#关闭其他通道输入
amixer sset 'Left Boost Mixer LINPUT1' off
amixer sset 'Left Input Boost Mixer LINPUT1' 0
#关闭麦克风左声道输入
amixer sset 'Left Boost Mixer LINPUT2' off 
amixer sset 'Left Input Boost Mixer LINPUT2' 0
#Line_in右声道输入关闭
amixer sset 'Left Boost Mixer LINPUT3' off
amixer sset 'Left Input Boost Mixer LINPUT3' 0


#音频输入，右声道管理
#Turn on Right Input Mixer Boost
amixer sset 'Right Input Mixer Boost' on
amixer sset 'Right Boost Mixer RINPUT1' off
amixer sset 'Right Input Boost Mixer RINPUT1' 0
amixer sset 'Right Boost Mixer RINPUT2' off
amixer sset 'Right Input Boost Mixer RINPUT2' 0

#要想设置成音频输入，请打开RINPUT3,看原理图可知
#其他的声道通过上面的配置可关闭，这样是为了避免干扰，需要的时候就打开
#RINPUT3打开（关键点）
amixer sset 'Right Boost Mixer RINPUT3' on
amixer sset 'Right Input Boost Mixer RINPUT3' 127

#!/bin/sh
#设置捕获的音量
amixer cset name='Capture Volume' 63,63
#音频输入，左声道管理
#Turn on Left Input Mixer Boost
amixer sset 'Left Input Mixer Boost' on
amixer sset 'Left Boost Mixer LINPUT1' on
amixer sset 'Left Input Boost Mixer LINPUT1' 127
amixer sset 'Left Boost Mixer LINPUT2' on 
amixer sset 'Left Input Boost Mixer LINPUT2' 127
#Turn off Left Boost Mixer LINPUT3
amixer sset 'Left Boost Mixer LINPUT3' off
amixer sset 'Left Input Boost Mixer LINPUT3' 0

#音频输入，右声道管理，全部关闭
#Turn on Right Input Mixer Boost
amixer sset 'Right Input Mixer Boost' off
amixer sset 'Right Boost Mixer RINPUT1' off
amixer sset 'Right Input Boost Mixer RINPUT2' 0
amixer sset 'Right Boost Mixer RINPUT2' off
amixer sset 'Right Input Boost Mixer RINPUT2' 0
amixer sset 'Right Boost Mixer RINPUT3' off
amixer sset 'Right Input Boost Mixer RINPUT3' 0

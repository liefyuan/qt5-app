#!/bin/sh
PATH="/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin"
#网口一测试，主要是测试它能不能获取正确的IP
ifconfig eth0 up
ifconfig eth1 down
#ifconfig lo down
sleep 2
#字符串截取处理
ipaddr=$(ifconfig |grep -E "inet addr:" |awk '{print $2}')
ip1=${ipaddr#*addr:}
ip1=${ip1:0:3}
ip2=192
#echo $ip1
#echo $ip2
echo "请插上网线现在正在测试ENET2，请稍等...，或者按KEY0跳过这项测试"
key_value1=$(cat /proc/interrupts |grep -E "USER-KEY1" |awk '{print $2}')
while [ "$ip1" != "$ip2" ]
do
sleep 1
ipaddr=$(ifconfig |grep -E "inet addr:" |awk '{print $2}')
ip1=${ipaddr#*addr:}
ip1=${ip1:0:3}
key_value2=$(cat /proc/interrupts |grep -E "USER-KEY1" |awk '{print $2}')
if [ $key_value1 != $key_value2  ]
then
      break
   fi
done

if [ "$ip1" == "$ip2" ] 
then
ENET2=ok
else
ENET2=erro
fi
sleep 2
#网口二测试，主要是测试它能不能获取正确的IP
ifconfig eth1 up
ifconfig eth0 down
sleep 2
ipaddr=$(ifconfig |grep -E "inet addr:" |awk '{print $2}')
ip1=${ipaddr#*addr:}
ip1=${ip1:0:3}
ip2=192
#echo $ip1
#echo $ip2
key_value1=$(cat /proc/interrupts |grep -E "USER-KEY1" |awk '{print $2}')
echo "请插上网线现在正在测试ENET1，请稍等...，或者按KEY0跳过这项测试"
while [ "$ip1" != "$ip2" ]
do
sleep 1
ipaddr=$(ifconfig |grep -E "inet addr:" |awk '{print $2}')
ip1=${ipaddr#*addr:}
ip1=${ip1:0:3}
key_value2=$(cat /proc/interrupts |grep -E "USER-KEY1" |awk '{print $2}')
if [ $key_value1 != $key_value2  ]
then
      break
   fi
done

if [ "$ip1" == "$ip2" ] 
then
ENET1=ok      
else
ENET1=erro
fi
#ifconfig lo up
ifconfig eth0 up
echo "****************"
echo "网口1结果:$ENET1"
echo "网口2结果:$ENET2"
echo "****************"
echo 1 > /sys/class/leds/beep/brightness
sleep 1
echo 0 > /sys/class/leds/beep/brightness
echo "请检查蜂鸣器是否响1s左右后关闭"

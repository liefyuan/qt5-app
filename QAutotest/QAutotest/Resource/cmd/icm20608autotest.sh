#!/bin/sh
PATH="/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin"
rmmod icm20608
insmod /opt/qt5.5.1/apps/QAutotest/Resource/driver/icm20608.ko
while [ 1 ]
do
/opt/qt5.5.1/apps/QAutotest/Resource/driver /dev/icm20608
sleep 1
done

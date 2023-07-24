#!/bin/sh

#/system/init/mac.sh

#/system/gpio/gpio_init.sh                          
#sleep 2

#ifconfig eth0 10.10.100.157 netmask 255.255.255.0
#route add default gw 10.10.100.250
#echo nameserver 10.10.100.250 > /tmp/resolv.conf

#udhcpc -i eth0 -q


ntpd -p time.windows.com -qNn &

/system/etc/mdev/sd_insert.sh

sleep 1

mkdir -p /mnt/sdcard2/log
#logcat -r 1024 -n 500 -f /mnt/sdcard2/log/log.txt &

#/usr/sbin/dropbear

#sleep 2

cp /system/init/setir  /tmp/

echo 1 > /proc/sys/vm/overcommit_memory

mount -t ubifs /dev/ubi1_0 /syskey
cp -f /syskey/device_info.ini /tmp/
umount /syskey

cd /system/init/
/system/init/start.sh start_param


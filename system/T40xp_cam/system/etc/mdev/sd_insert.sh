#!/bin/sh
echo "@@@@@@@@ sd card insert! @@@@@@@@" > /dev/console
if [ -e "/dev/mmcblk0p1" ]; then
mkdir /mnt/sdcard
mount -t vfat /dev/mmcblk0p1 /mnt/sdcard -o rw,errors=continue
fi
if [ -e "/dev/mmcblk0p2" ]; then
mkdir /mnt/sdcard2
mount -t vfat /dev/mmcblk0p2 /mnt/sdcard2 -o rw,errors=continue
fi


#!/bin/sh
echo "@@@@@@@@ sd card remove! @@@@@@@@" > /dev/console
umount -l /mnt/sdcard*
umount -l /system/sdcard2*
rm –rf /mnt/sdcard*
rm -rf /system/sdcard2*


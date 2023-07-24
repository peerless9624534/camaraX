#!/bin/sh

mkdir -p /system/nfs
mkdir -p /system/tmp

umount /system/nfs
umount /system/tmp

mount -t nfs -o nolock 10.10.100.155:/home/humy/samba /system/tmp
mount -t nfs -o nolock 10.10.100.156:/LX_work /system/nfs

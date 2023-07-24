#!/bin/bash

KEYFS_UBI_CFG=./keyubi.cfg
KEYFS_UBI_FILE=keyubi.ubifs
KEYFS_IMG_FILE=keyubi.img

SYSTEMFS_PATH=key_tmp

if [ -f ./${KEYFS_UBI_FILE} ]; then
	rm ./${KEYFS_UBI_FILE}
fi

if [ -f ./${KEYFS_IMG_FILE} ]; then
	rm ./${KEYFS_IMG_FILE}
fi

rm -rf ./key_tmp
cp -rf ./key ./key_tmp

find ./key_tmp -type f -print0 | xargs -0 dos2unix

sleep 1
sleep 1
sleep 1
sleep 1
sleep 1

fakeroot mkfs.ubifs -e 0x1f000 -c 48 -m 2048 -d ${SYSTEMFS_PATH}/ -o ${KEYFS_UBI_FILE} -v

rm -rf ./system_tmp

rm -rf ${KEYFS_UBI_CFG}
touch  ${KEYFS_UBI_CFG}

echo "[ubifs]"               >> ${KEYFS_UBI_CFG}
echo "mode=ubi"              >> ${KEYFS_UBI_CFG}
echo "image=keyubi.ubifs"    >> ${KEYFS_UBI_CFG}
echo "vol_id=0"              >> ${KEYFS_UBI_CFG}
echo "vol_size=4MiB"         >> ${KEYFS_UBI_CFG}
echo "vol_type=dynamic"      >> ${KEYFS_UBI_CFG}
echo "vol_name=key"          >> ${KEYFS_UBI_CFG}
echo "vol_flags=autoresize"  >> ${KEYFS_UBI_CFG}
echo "vol_alignment=1"       >> ${KEYFS_UBI_CFG}

fakeroot ubinize -o ${KEYFS_IMG_FILE} -m 2048 -p 128KiB -s 2048 ${KEYFS_UBI_CFG} -v

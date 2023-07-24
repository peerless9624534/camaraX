#!/bin/bash

SYSTEMFS_UBI_CFG=./systemubi.cfg
SYSTEMFS_RESTORE_UBI_CFG=./systemubi_restore.cfg
SYSTEMFS_UBI_FILE=systemubi.ubifs
SYSTEMFS_IMG_FILE=systemubi.img
RESTORE_SYSTEMFS_UBI_FILE=restoresystemubi.ubifs
SYSTEMFS_RESTORE_IMG_FILE=systemubi_restore.img
SYSTEMFS_PATH=system_tmp

if [ -f ./${SYSTEMFS_UBI_FILE} ]; then
	rm ./${SYSTEMFS_UBI_FILE}
fi

if [ -f ./${SYSTEMFS_IMG_FILE} ]; then
	rm ./${SYSTEMFS_IMG_FILE}
fi

#dos2unix ./system/init/*.sh ./system/init/*.ini ./system/jio/*.ini 
#dos2unix ./system/gpio/*.sh ./system/gpio/ircut/* ./system/gpio/irlignt/* ./system/gpio/led/* ./system/rest/* ./system/sd/* ./system/speak/* ./system/wifi/*

rm -rf ./system_tmp
cp -rf ./system ./system_tmp

if [ "$1" = "bullet" ]; then
	mv ./system_tmp/config/luxconfig_bullet.ini ./system_tmp/config/luxconfig.ini
	echo "***************************************************"
	echo "****             bullet              **************"
	echo "***************************************************"
elif [ "$1" = "dome" ]; then
	mv ./system_tmp/config/luxconfig_dome.ini ./system_tmp/config/luxconfig.ini
	echo "***************************************************"
	echo "****             dome                **************"
	echo "***************************************************"
else
	mv ./system_tmp/config/luxconfig_bullet.ini ./system_tmp/config/luxconfig.ini
	echo "***************************************************"
	echo "****          default for bullet     **************"
	echo "***************************************************"
fi

cat ./system_tmp/config/luxconfig.ini | grep -E "lowthreshold|highthreshold"

find ./system_tmp -type f -print0 | xargs -0 dos2unix

sleep 1
sleep 1
sleep 1
sleep 1
sleep 1

rm -rf ./system_tmp/.system
touch ./system_tmp/.system

mkdir -p ./system_tmp/.ssh

rm -rf ${SYSTEMFS_UBI_FILE}
cp -f ./system_tmp/network/interfaces_static ./system_tmp/network/interfaces
fakeroot mkfs.ubifs -e 0x1f000 -c 416 -m 2048 -d ${SYSTEMFS_PATH}/ -o ${SYSTEMFS_UBI_FILE} -v

rm -rf ${SYSTEMFS_UBI_CFG}
touch  ${SYSTEMFS_UBI_CFG}

echo "[ubifs]" >> ${SYSTEMFS_UBI_CFG}
echo "mode=ubi" >> ${SYSTEMFS_UBI_CFG}
echo "image=systemubi.ubifs" >> ${SYSTEMFS_UBI_CFG}
echo "vol_id=0" >> ${SYSTEMFS_UBI_CFG}
echo "vol_size=46MiB" >> ${SYSTEMFS_UBI_CFG}
echo "vol_type=dynamic" >> ${SYSTEMFS_UBI_CFG}
echo "vol_name=system" >> ${SYSTEMFS_UBI_CFG}
echo "vol_flags=autoresize" >> ${SYSTEMFS_UBI_CFG}
echo "vol_alignment=1" >> ${SYSTEMFS_UBI_CFG}

fakeroot ubinize -o ${SYSTEMFS_IMG_FILE} -m 2048 -p 128KiB -s 2048 ${SYSTEMFS_UBI_CFG} -v

rm -rf ${RESTORE_SYSTEMFS_UBI_FILE}
cp -f  ./system_tmp/network/interfaces_dhcp ./system_tmp/network/interfaces
rm -rf ./system_tmp/init/start.sh
mv ./system_tmp/init/start_restore.sh ./system_tmp/init/start.sh
fakeroot mkfs.ubifs -e 0x1f000 -c 368 -m 2048 -d ${SYSTEMFS_PATH}/ -o ${RESTORE_SYSTEMFS_UBI_FILE} -v

rm -rf ${SYSTEMFS_RESTORE_UBI_CFG}
touch  ${SYSTEMFS_RESTORE_UBI_CFG}

echo "[ubifs]" >> ${SYSTEMFS_RESTORE_UBI_CFG}
echo "mode=ubi" >> ${SYSTEMFS_RESTORE_UBI_CFG}
echo "image=restoresystemubi.ubifs" >> ${SYSTEMFS_RESTORE_UBI_CFG}
echo "vol_id=0" >> ${SYSTEMFS_RESTORE_UBI_CFG}
echo "vol_size=41MiB" >> ${SYSTEMFS_RESTORE_UBI_CFG}
echo "vol_type=dynamic" >> ${SYSTEMFS_RESTORE_UBI_CFG}
echo "vol_name=restore_system" >> ${SYSTEMFS_RESTORE_UBI_CFG}
echo "vol_flags=autoresize" >> ${SYSTEMFS_RESTORE_UBI_CFG}
echo "vol_alignment=1" >> ${SYSTEMFS_RESTORE_UBI_CFG}

fakeroot ubinize -o ${SYSTEMFS_RESTORE_IMG_FILE} -m 2048 -p 128KiB -s 2048 ${SYSTEMFS_RESTORE_UBI_CFG} -v

rm -rf ./system_tmp


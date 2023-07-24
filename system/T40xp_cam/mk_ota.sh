#!/bin/bash

rm -rf ./ota_system.sh

echo '#!/bin/bash' >> ota_system.sh
echo 'programs=$(ps -l | grep "reset_key.sh" | grep -v "grep" | awk '"'"'{print $3}'"'"')'  >> ota_system.sh
echo 'kill -9 "$programs"' >> ota_system.sh

echo 'programs=$(ps -l | grep "onvif" | grep -v "grep" | awk '"'"'{print $3}'"'"')'         >> ota_system.sh
echo 'kill -9 "$programs"' >> ota_system.sh

echo 'programs=$(ps -l | grep "restart.sh" | grep -v "grep" | awk '"'"'{print $3}'"'"')'    >> ota_system.sh
echo 'kill -9 "$programs"' >> ota_system.sh

echo 'programs=$(ps -l | grep "media_sample" | grep -v "grep" | awk '"'"'{print $3}'"'"')'  >> ota_system.sh
echo 'kill -9 "$programs"' >> ota_system.sh

echo 'rm -rf ./system' >> ota_system.sh

echo 'mv ./system_tmp ./system' >> ota_system.sh

echo 'rm -rf /system/bin;rm -rf /system/etc;rm -rf /system/gpio;rm -rf /system/init;rm -rf /system/lib' >> ota_system.sh

echo "cp -rf ./system/bin /system/bin; cp -rf ./system/etc /system/etc; cp -rf ./system/gpio /system/gpio; cp -rf ./system/init /system/init; cp -rf ./system/lib /system/lib; sync; sync; sync" >> ota_system.sh

echo 'chmod -R 777 /system/bin;chmod 777 /system/etc;chmod 777 /system/gpio;chmod 777 /system/init;chmod 777 /system/lib' >> ota_system.sh

echo 'sync; sync; sync' >> ota_system.sh

echo 'reboot ' >> ota_system.sh

sync
sleep 1
sync
sleep 1

rm -rf ./system_tmp
cp -rf ./system ./system_tmp

find ./system_tmp -type f -print0 | xargs -0 dos2unix

rm -rf ota_system.zip

zip -vr ota_system.zip ./ota_system.sh ./system_tmp/bin ./system_tmp/etc ./system_tmp/gpio ./system_tmp/init ./system_tmp/lib

rm -rf ota_system.sh
rm -rf ./system_tmp

#===========================================================================================

rm -rf ota_restore.sh

echo '#!/bin/bash'  >>  ota_restore.sh

echo 'programs=$(ps -l | grep "reset_key.sh" | grep -v "grep" | awk '"'"'{print $3}'"'"')'  >> ota_restore.sh
echo 'kill -9 "$programs"' >> ota_restore.sh

echo 'rm -rf ./restore_system' >> ota_restore.sh

echo 'mv ./restore_system_tmp ./restore_system' >> ota_restore.sh

echo 'umount /restore_system' >> ota_restore.sh

echo 'mount -t ubifs /dev/ubi2_0 /restore_system' >> ota_restore.sh

echo 'rm -rf /restore_system/network;rm -rf /restore_system/config;rm -rf /restore_system/bin;rm -rf /restore_system/etc;rm -rf /restore_system/gpio;rm -rf /restore_system/init;rm -rf /restore_system/lib' >> ota_restore.sh

echo "cp -rf ./restore_system/network /restore_system/network;cp -rf ./restore_system/config /restore_system/config;cp -rf ./restore_system/bin /restore_system/bin; cp -rf ./restore_system/etc /restore_system/etc; cp -rf ./restore_system/gpio /restore_system/gpio; cp -rf ./restore_system/init /restore_system/init; cp -rf ./restore_system/lib /restore_system/lib; sync; sync; sync" >> ota_restore.sh

echo 'chmod -R 777 /restore_system/network;chmod -R 777 /restore_system/config;chmod -R 777 /restore_system/bin;chmod 777 /restore_system/etc;chmod 777 /restore_system/gpio;chmod 777 /restore_system/init;chmod 777 /restore_system/lib' >> ota_restore.sh

echo 'sync; sync; sync' >> ota_restore.sh

echo 'reboot ' >> ota_restore.sh

sync
sleep 1
sync
sleep 1

rm -rf ./restore_system_tmp
cp -rf ./system ./restore_system_tmp

cp -f  ./restore_system_tmp/init/restore_start.sh ./restore_system_tmp/init/start.sh
cp -f  ./restore_system_tmp/network/interfaces_dhcp ./restore_system_tmp/network/interfaces

find ./restore_system_tmp -type f -print0 | xargs -0 dos2unix

rm -rf ota_restore.zip

zip -vr ota_restore.zip ./ota_restore.sh ./restore_system_tmp/network ./restore_system_tmp/config ./restore_system_tmp/bin ./restore_system_tmp/etc ./restore_system_tmp/gpio ./restore_system_tmp/init ./restore_system_tmp/lib

rm -rf ota_restore.sh
rm -rf ./restore_system_tmp

#===========================================================================================

rm -rf ota_key.sh

echo '#!/bin/bash'  >>  ota_key.sh

echo 'rm -rf  ./key' >> ota_key.sh

echo 'mv ./key_tmp  ./key' >> ota_key.sh

echo 'umount /syskey' >> ota_key.sh

echo 'mount -t ubifs /dev/ubi1_0 /syskey' >> ota_key.sh

echo 'rm -rf /syskey/device_info.ini' >> ota_key.sh

echo 'cp -f ./key/device_info.ini /syskey/' >> ota_key.sh

echo 'chmod 777 /syskey/device_info.ini' >> ota_key.sh

echo 'umount /syskey' >> ota_key.sh

echo 'sync; sync; sync' >> ota_key.sh

echo 'reboot ' >> ota_key.sh

rm -rf ./key_tmp

cp -rf ./key ./key_tmp

find ./key_tmp -type f -print0 | xargs -0 dos2unix

rm -rf ota_key.zip

zip -vr ota_key.zip ./ota_key.sh ./key_tmp

rm -rf ota_key.sh
rm -rf ./key_tmp

#!/bin/bash

echo $PATH

#./mk_clean
mkdir -p out
#cd main;make CONFIG_PTZ=n CONFIG_BABY=y CONFIG_RTSP=y
cd main;make CONFIG_PTZ=n CONFIG_BABY=y CONFIG_JIO_DEMO=y
# cd main;make CONFIG_PTZ=y CONFIG_BABY=y CONFIG_JIO_SKEL=y
# cd main;make CONFIG_PTZ=y CONFIG_BABY=y CONFIG_RTSP=y CONFIG_UCLIBC=y
# cd main;make CONFIG_PTZ=y CONFIG_BABY=y CONFIG_JIO_DEMO=y CONFIG_UCLIBC=y
# cd main;make CONFIG_PTZ=y CONFIG_BABY=y CONFIG_JIO_SKEL=y CONFIG_UCLIBC=y
cd ../daemon/;make
cd ../reset_key/;make
cd ../ota/;make
cd ..

chmod 777 out/ -R
#cp -p out/media_sample out/daemon out/reset_key out/ota ../system/babycam/system/bin/

#cp -p out/media_sample out/daemon out/reset_key out/ota ../system/babycamEU/system/bin/

#cp out/media_sample out/media_sample.bin
#chmod +x out/media_sample.bin
#cp -rf out/media_sample /home/mc/Nfs

cp -rf out/media_sample /home/nfs

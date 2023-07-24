#!/bin/bash

# if [ -d "build" ]; then
#     rm -rf  build
# fi
# mkdir -p  build

# if [ -f "build/log.c" ]; then
#     rm -rf  build/log.c
# fi
# #     cd build
    # cmake  ../camera_cdb/main -DCMAKE_BUILD_TYPE=Debug
#     cmake  --build . 2>&1 | tee log.c   
    make -j8

# cp -rf  out/xcamera  /mnt/d/wsj/tool/

cp -rf  media_sample  /mnt/d/wsj/tool/app
if [ $? -eq 0 ]; then
    echo "Command succeeded."
else
    echo "Command failed with exit code $?."
fi
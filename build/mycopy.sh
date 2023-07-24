#!/bin/bash
set +v
if [ -f /home/sj/camaraX/camera_cdb/main/copybin.sh ]; then
    chmod 777 /home/sj/camaraX/camera_cdb/main/copybin.sh
    /home/sj/camaraX/camera_cdb/main/copybin.sh
fi
set -v

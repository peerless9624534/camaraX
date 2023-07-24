#!/bin/bash

if [ -f ./system.jffs2 ]; then
	rm cam_t40.img system.jffs2
fi

if [ ! -f ./rootfs.squashfs ]; then
	echo "rootfs.squashfs is not exists, please check it!"
	exit
fi

if [ ! -f ./uImage ]; then
	echo "uImage is not exists, please check it!"
	exit
fi

if [ ! -f ./u-boot-with-spl.bin ]; then
	echo "u-boot-with-spl.bin is not exists, please check it!"
	exit
fi

rm -rf ./system/.system
touch ./system/.system

UBOOT_PART_SIZE=$((256 * 1024))
KERNEL_PART_SIZE=$((2560 * 1024))
ROOTFS_PART_SIZE=$((2432 * 1024))
KEY_PART_SIZE=$((256 * 1024))
SIZE_8M=$((8192 * 1024))
SIZE_16M=$((16384 * 1024))
SIZE_128M=$((131072 * 1024))
SYSTEMFS_8M=$((${SIZE_8M}-${UBOOT_PART_SIZE}-${KERNEL_PART_SIZE}-${ROOTFS_PART_SIZE}-${KEY_PART_SIZE}))
SYSTEMFS_16M=$((${SIZE_16M}-${UBOOT_PART_SIZE}-${KERNEL_PART_SIZE}-${ROOTFS_PART_SIZE}-${KEY_PART_SIZE}))
SYSTEMFS_128M=$((${SIZE_128M}-${UBOOT_PART_SIZE}-${KERNEL_PART_SIZE}-${ROOTFS_PART_SIZE}-${KEY_PART_SIZE}))
SYSTEMFS_PART_SIZE=${SYSTEMFS_16M}

fakeroot mkfs.jffs2 -d ./system -l -e 0x8000 -s 0x1000 -o system.jffs2 --pad=${SYSTEMFS_PART_SIZE}
#fakeroot mkfs.jffs2 -o system.jffs2 -r system -e 0x8000 -s 0x1000 -n -l -X zlib --pad=${SYSTEMFS_PART_SIZE}
#fakeroot mkfs.jffs2 -d ./system -l -e 0x8000 -s 0x1000 -o system.jffs2
sleep 1

dd if=u-boot-with-spl.bin of=cam_t40.img obs=1k seek=0
dd if=uImage 			  of=cam_t40.img obs=${UBOOT_PART_SIZE} seek=1
dd if=rootfs.squashfs 	  of=cam_t40.img obs=$((${UBOOT_PART_SIZE}+${KERNEL_PART_SIZE})) seek=1
dd if=tuya_uuid_key.ini   of=cam_t40.img obs=$((${UBOOT_PART_SIZE}+${KERNEL_PART_SIZE}+${ROOTFS_PART_SIZE})) seek=1
dd if=system.jffs2 		  of=cam_t40.img obs=$((${UBOOT_PART_SIZE}+${KERNEL_PART_SIZE}+${ROOTFS_PART_SIZE}+${KEY_PART_SIZE})) seek=1

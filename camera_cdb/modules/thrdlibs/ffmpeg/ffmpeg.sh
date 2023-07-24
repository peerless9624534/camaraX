rm -rf $(pwd)/out/include/*
rm -rf $(pwd)/out/lib/*
rm -rf $(pwd)/../../inc/ffmpeg/*
rm -rf $(pwd)/../../lib/ffmpeg/*
rm -rf /home/mc/Nfs/output/include/libav*
rm -rf /home/mc/Nfs/output/include/libpo*
rm -rf /home/mc/Nfs/output/include/libsw*
rm -rf /home/mc/Nfs/output/lib/libav*
rm -rf /home/mc/Nfs/output/lib/libpo*
rm -rf /home/mc/Nfs/output/lib/libsw*
echo "***************************clean success***************************************"

./configure \
--pkg-config=true \
--prefix=$(pwd)/out \
--enable-cross-compile --arch=mips --target-os=linux \
--cross-prefix=mips-linux-gnu- --cc=mips-linux-gnu-gcc-4.7.2 \
--extra-cflags=-I$(pwd)/../x264/out/include \
--extra-ldflags=-L$(pwd)/../x264/out/lib \
--extra-libs=-ldl \
--enable-libx264 \
--enable-gpl \
--disable-everything \
--disable-x86asm \
--disable-mipsdsp --disable-mipsdspr2 --disable-mips32r2 --disable-mipsfpu \
--disable-autodetect \
--disable-avdevice \
--enable-avfilter \
--disable-msa \
--disable-debug \
--disable-protocols --enable-protocol=file \
--disable-demuxers --enable-demuxer=mov \
--disable-muxers --enable-muxer=mp4 --enable-muxer=mov \
--disable-decoders --enable-decoder=pcm_alaw --enable-decoder=h264  --enable-decoder=mpeg4 \
--disable-encoders --enable-encoder=pcm_s16le \
--disable-bsfs --enable-bsf=h264_mp4toannexb \
--enable-static --disable-shared \
--enable-small

#--extra-cflags=-I/home/mc/Nfs/output/include \
#--extra-ldflags=-L/home/mc/Nfs/output/lib \
#--disable-static --enable-shared \
#CFLAGS="-g -O2 -Wall -fPIC" 
#CXXFLAGS="-g -O2 -Wall -fPIC"
#CFLAGS="-fPIC" 
#CXXFLAGS="-fPIC"

#make -j32
make -j16
make install

#cp -rf  /home/mc/Nfs/output/include/libav* \
#/home/mc/Nfs/output/include/libp* \
#/home/mc/Nfs/output/include/libs* \
#/home/mc/Toolchain/mips-gcc472-glibc216-64bit/mips-linux-gnu/libc/usr/include

if [[ $?==0 ]];then
	cp -rf $(pwd)/out/include/* $(pwd)/../../inc/ffmpeg
	cp -rf $(pwd)/out/lib/* $(pwd)/../../lib/ffmpeg
	cp -rf $(pwd)/out/include/* /home/mc/Nfs/output/include
	cp -rf $(pwd)/out/lib/* /home/mc/Nfs/output/lib
	echo "********************Make Install Success**********************"
else
	echo "********************Make Install Failed**********************"
fi


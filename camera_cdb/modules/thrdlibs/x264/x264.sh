rm -rf $(pwd)/out/include/*
rm -rf $(pwd)/out/lib/*
rm -rf $(pwd)/../../inc/x264/*
rm -rf $(pwd)/../../lib/x264/*
rm -rf /home/mc/Nfs/output/include/x264*.h
rm -rf /home/mc/Nfs/output/lib/libx264.*
echo "***************************clean success***************************************"

./configure --prefix=$(pwd)/out \
--cross-prefix=/home/mc/Toolchain/mips-gcc472-glibc216-64bit/bin/mips-linux-gnu- \
--disable-asm \
--host=mips-linux \
--disable-opencl \
--disable-cli \
--enable-pic \
--enable-static \
--enable-shared  

#CFLAGS="-g -O2 -Wall -fPIC" 
#CXXFLAGS="-g -O2 -Wall -fPIC"
#CFLAGS="-fPIC" 
#CXXFLAGS="-fPIC"

#make -j32
make -j16
make install
if [[ $?==0 ]];then
	cp -rf $(pwd)/out/include/* $(pwd)/../../inc/x264/
	cp -rf $(pwd)/out/lib/* $(pwd)/../../lib/x264
	cp -rf $(pwd)/out/include/* /home/mc/Nfs/output/include
	cp -rf $(pwd)/out/lib/* /home/mc/Nfs/output/lib
	echo "********************Make Install Success**********************"
else
	echo "********************Make Install Failed**********************"
fi

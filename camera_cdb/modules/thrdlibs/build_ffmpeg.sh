set -e

echo "start compile ffmpeg"
#sudo add-apt-repository ppa:tortoisehg-ppa/releases 
#sudo add-apt-repository ppa:mercurial-ppa/releases 
#sudo apt-get update 
sudo apt-get install mercurial python-nautilus tortoisehg 
echo "-------------->install mercurial python-nautilus tortoisehg successfully"
sudo apt install mercurial
echo "-------------->install mercurial successfully"
sudo apt-get install git
echo "-------------->install git successfully"
sudo apt-get install autoconf automake libtool
echo "-------------->install autoconf automake libtool successfully"

# create ffmpeg dir
if [ ! -d "$HOME/ffmpeg_sources" ]; then
	 mkdir ~/ffmpeg_sources
 fi

 if [ ! -d "$HOME/ffmpeg_build" ]; then
	  mkdir ~/ffmpeg_build
  fi



  # install nasm
  cd ~/ffmpeg_sources
  wget http://www.nasm.us/pub/nasm/releasebuilds/2.13.02/nasm-2.13.02.tar.bz2 
  tar xjvf nasm-2.13.02.tar.bz2
  cd nasm-2.13.02
  ./autogen.sh 
  ./configure 
  make
  sudo make install
  echo "-------------->install nasm successfully"

  # install yasm
  cd ~/ffmpeg_sources
  wget -O yasm-1.3.0.tar.gz http://www.tortall.net/projects/yasm/releases/yasm-1.3.0.tar.gz
  tar xzvf yasm-1.3.0.tar.gz
  cd yasm-1.3.0
  ./configure
  make
  sudo make install
  echo "-------------->install yasm successfully"

  # libx264
  cd ~/ffmpeg_sources
  git -C x264 pull 2> /dev/null || git clone --depth 1 https://git.videolan.org/git/x264.git
  cd x264
  ./configure --enable-static --disable-opencl --extra-cflags="-fPIC"
  make
  sudo make install
  echo "-------------->install libx264 successfully"


  # libfdk-aac
  cd ~/ffmpeg_sources
  wget https://nchc.dl.sourceforge.net/project/opencore-amr/fdk-aac/fdk-aac-0.1.6.tar.gz
  tar -zxvf fdk-aac-0.1.6.tar.gz
  cd fdk-aac-0.1.6
  autoreconf 
  ./configure
  make
  sudo make install
  echo "-------------->install libfdk-aac successfully"

  # libmp3lame
  cd ~/ffmpeg_sources
  wget -O lame-3.100.tar.gz http://downloads.sourceforge.net/project/lame/lame/3.100/lame-3.100.tar.gz
  tar xzvf lame-3.100.tar.gz
  cd lame-3.100
  ./configure  
  make 
  sudo make install
  echo "-------------->install libmp3lame successfully"

  # install sdl2
  cd ~/ffmpeg_sources
  wget -O SDL2-2.0.6.zip http://libsdl.org/release/SDL2-2.0.6.zip
  unzip SDL2-2.0.6.zip
  cd SDL2-2.0.6
  ./configure --enable-static
  make
  sudo make install
  echo "-------------->install sdl2 successfully"

  # FFmpeg
  cd ~/ffmpeg_sources 
  wget  https://ffmpeg.org/releases/ffmpeg-4.0.2.tar.bz2
  tar -jxvf ffmpeg-4.0.2.tar.bz2
  cd ffmpeg-4.0.2
  ./configure --extra-cflags="-I/usr/local/include" --extra-ldflags="-L/usr/local/lib" --extra-cflags="-fPIC" --enable-gpl  --enable-nonfree --enable-libx264   --enable-debug --enable-postproc   --disable-optimizations --enable-stripping --enable-pthreads  --enable-shared --enable-libfdk-aac --enable-libmp3lame --enable-pic --enable-version3  --enable-sdl2
  make
  sudo make install

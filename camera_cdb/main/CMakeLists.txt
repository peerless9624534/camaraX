#****************************************************************
#     CMake 文件, author: humy  2023.3.22
#     Debug compile: cmake ../main -DCMAKE_BUILD_TYPE=Debug 
#     Release compile: cmake ../main -DCMAKE_BUILD_TYPE=Release
#****************************************************************

# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.10.0)

project(media_sample CXX C)

message(STATUS "current CMAKE_VERSION=${CMAKE_VERSION}")

message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

#add_compile_options()
#add_custom_command()
#add_link_options()
#add_definitions()
#add_compile_definitions()
#add_custom_target()
#add_dependencies()
#add_executable()
#add_library()
#add_subdirectory()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/cmake_config.h")

option(CONFIG_JIO_DEMO  "=======application connect to jio cloud!"   ON)

option(CONFIG_RTSP      "=======application is rtsp server!"         ON)

option(PLATFORM_T31     "=======Embedded system is Ingenic T31N"     OFF)

option(PLATFORM_T40     "=======Embedded system is Ingenic T40XP"    ON)

option(deploy           "This is a option for deploy"                OFF)

option(CONFIG_BABY      "=======application is for baby monitor"     ON) 

if (${CONFIG_JIO_DEMO} STREQUAL "ON")
    add_compile_definitions(CONFIG_JIO_DEMO)
    message("====== add_compile_definitions(CONFIG_JIO_DEMO)")
endif()

if (${CONFIG_RTSP} STREQUAL "ON")
    add_compile_definitions(CONFIG_RTSP)
    message("====== add_compile_definitions(CONFIG_RTSP)")
endif()

if (${PLATFORM_T40} STREQUAL "ON")
    add_compile_definitions(PLATFORM_T40)
    message("====== add_compile_definitions(PLATFORM_T40)")
else ()
    add_compile_definitions(PLATFORM_T31)
    message("====== add_compile_definitions(PLATFORM_T31)")
endif()

add_compile_definitions(CONFIG_BABY)
message("====== add_compile_definitions(CONFIG_BABY)")

add_compile_definitions(WITH_DOM)
message("====== add_compile_definitions(WITH_DOM)")

add_compile_definitions(WITH_OPENSSL)
message("====== add_compile_definitions(WITH_OPENSSL)")

add_compile_definitions(__LINUX__)

#指定 C++ std11
set(CMAKE_CXX_STANDARD 11)

#指定项目名称
SET(PROJECT_NAME "media_sample")

#指定项目的输出名称
SET(BIN_NAME "media_sample")

#设置解决方案工作路径
set (SOLUTION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../)

#设置解决方案名称
if (NOT SOLUTION_NAME)
    set (SOLUTION_NAME ${PROJECT_NAME})
endif()

#find_package(CUDA 5.5 QUIET)
#message(STATUS "CUDA detected: " ${CUDA_VERSION})

#指定执行文件输出路径 
#SET(EXECUTABLE_OUTPUT_PATH ~/project_out/${SOLUTION_NAME}/bin/LINUX64)

#设置库文件输出路径
#SET(LIBRARY_OUTPUT_PATH ~/project_out/${SOLUTION_NAME}/bin/${PROJECT_NAME}/LINUX64)
    
#设置工程工作路径
set (PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#设置PUBLICMODULES的路径
#set (PUBLIC_MODULES_DIR ${SOLUTION_DIR}/../../../publicModules)

#set(USR_LOCAL "/usr/local")

# turn on C++11
#if(CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -w")
#endif()

SET(CROSS_COMPILE mips-linux-gnu-)

set(CMAKE_C_COMPILER        "${CROSS_COMPILE}gcc")
set(CMAKE_CXX_COMPILER      "${CROSS_COMPILE}g++")
set(CMAKE_AR                "${CROSS_COMPILE}ar")
set(CMAKE_AS                "${CROSS_COMPILE}as")
set(CMAKE_LD                "${CROSS_COMPILE}ld")
set(CMAKE_NM                "${CROSS_COMPILE}nm")
set(CMAKE_STRIP             "${CROSS_COMPILE}strip")
set(CMAKE_OBJCOPY           "${CROSS_COMPILE}objcopy")
set(CMAKE_OBJDUMP           "${CROSS_COMPILE}objdump")

SET(CMAKE_C_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -O0 -march=mips32r2 -g -ggdb") 
#SET(CMAKE_C_FLAGS_RELEASE "$ENV{CXXFLAGS} -Wall -O2 -march=mips32r2") 
SET(CMAKE_C_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable -march=mips32r2 -s")
SET(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -O2 -Wall -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable -march=mips32r2 -s -pthread")
SET(CMAKE_CXX_FLAGS "$ENV{CMAKE_CXX_FLAGS} -std=c++11 -pthread")

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wall -Wl,--gc-section")

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    set(CMAKE_C_FLAGS "$ENV{CMAKE_C_FLAGS_DEBUG}")
    message("Build mode: Debug")
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    set(CMAKE_C_FLAGS "$ENV{CMAKE_C_FLAGS_RELEASE}")
    message("Build mode: Release")
else()
    set(CMAKE_C_FLAGS  "$ENV{CMAKE_C_FLAGS_RELEASE}")
    message("Build mode: Default release")
endif()


if(${deploy} STREQUAL "ON")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../out/)
endif()


SET(BUILD_ROOT           ${CMAKE_CURRENT_SOURCE_DIR}/..)
SET(ALG_LIB_DIR          ${BUILD_ROOT}/libs/7.2.0_1.2.0/ingenic/glibc/alg)
SET(SDK_LIB_DIR          ${BUILD_ROOT}/libs/7.2.0_1.2.0/ingenic/glibc)
SET(JIO_LIB_DIR          ${BUILD_ROOT}/libs/7.2.0_1.2.0/jio/jio_with_openssl)
#SET(JIO_LIB_DIR          ${BUILD_ROOT}/libs/7.2.0_1.2.0/jio/jio_with_mbedtls)
# SET(ALG_COMMON_LIB_DIR   ${BUILD_ROOT}/libs/7.2.0_1.2.0/ingenic/alg)
SET(THIRD_LIB_DIR        ${BUILD_ROOT}/libs/7.2.0_1.2.0/third)

#包含头文件路径
include_directories(${BUILD_ROOT}/)
include_directories(${BUILD_ROOT}/common/include/)
include_directories(${BUILD_ROOT}/hal/include/)
include_directories(${BUILD_ROOT}/main/)
include_directories(${BUILD_ROOT}/modules/ingenic/1.2.0/imp/include/)
include_directories(${BUILD_ROOT}/modules/ingenic/1.2.0/sysutils/include/)
include_directories(${BUILD_ROOT}/modules/media/include/)
include_directories(${BUILD_ROOT}/modules/rtsp/include/)
include_directories(${BUILD_ROOT}/modules/Users/include)
include_directories(${BUILD_ROOT}/modules/thrdlibs/zlib/include/)
include_directories(${BUILD_ROOT}/modules/thrdlibs/zbar/include/)
#include_directories(${BUILD_ROOT}/modules/thrdlibs/rtsp/)
#include_directories(${BUILD_ROOT}/modules/thrdlibs/rtspnew/)
include_directories(${BUILD_ROOT}/modules/thrdlibs/iniparse/include/)
include_directories(${BUILD_ROOT}/modules/opencv_mips_7.2.0_2.29/include)
include_directories(${BUILD_ROOT}/modules/fdk-aac/include/)
include_directories(${BUILD_ROOT}/hal/wifi/include/)
include_directories(${BUILD_ROOT}/hal/led/include/)
include_directories(${BUILD_ROOT}/hal/misc/include/)
include_directories(${BUILD_ROOT}/hal/reset_key/include/)
include_directories(${BUILD_ROOT}/modules/ingenic/1.2.0/alg/include/)
include_directories(${BUILD_ROOT}/modules/ingenic/1.2.0/ivs/include/)
include_directories(${BUILD_ROOT}/hal/motor/include/)
include_directories(${BUILD_ROOT}/hal/i2c/include/)
include_directories(${BUILD_ROOT}/main/jio_demo/inc/)
include_directories(${BUILD_ROOT}/main/jio_demo/inc/x264/)
include_directories(${BUILD_ROOT}/main/jio_demo/inc/ffmpeg/)
include_directories(${BUILD_ROOT}/modules/jio/include/)
include_directories(${BUILD_ROOT}/modules/jio/include/libwebsockets-build/)
include_directories(${BUILD_ROOT}/modules/jio/include/idip/)
include_directories(${BUILD_ROOT}/modules/jio/include/idip/details/)
include_directories(${BUILD_ROOT}/main/rtsp_demo/include/)
include_directories(${BUILD_ROOT}/main/lux_server/include/)
include_directories(${BUILD_ROOT}/modules/onvif/inc/)
include_directories(${BUILD_ROOT}/modules/onvif/src/)
include_directories(${BUILD_ROOT}/modules/onvif/src_soap_new_2/)
include_directories(${BUILD_ROOT}/modules/openssl/include)
include_directories(${BUILD_ROOT}/modules/config/include)
include_directories(${BUILD_ROOT}/modules/factory_test/inc/)
include_directories(${BUILD_ROOT}/modules/thrdlibs/rtsp2/inc)


link_directories(${ALG_LIB_DIR})



SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 
SET(CMAKE_INSTALL_RPATH "/system/bin")

#设置编译源文件列表
aux_source_directory(${BUILD_ROOT}/common/src                         SRC_FILES)
aux_source_directory(${BUILD_ROOT}/modules/ingenic/1.2.0/imp/src      SRC_FILES)
aux_source_directory(${BUILD_ROOT}/modules/ingenic/1.2.0/sysutils/src SRC_FILES)
aux_source_directory(${BUILD_ROOT}/modules/media/src                  SRC_FILES)
aux_source_directory(${BUILD_ROOT}/modules/rtsp/src                   SRC_FILES)
aux_source_directory(${BUILD_ROOT}/modules/Users/src                  SRC_FILES)
aux_source_directory(${BUILD_ROOT}/modules/config/src                 SRC_FILES)
aux_source_directory(${BUILD_ROOT}/modules/thrdlibs/zlib/src          SRC_FILES)
#aux_source_directory(${BUILD_ROOT}/modules/thrdlibs/rtsp              SRC_FILES)
#aux_source_directory(${BUILD_ROOT}/modules/thrdlibs/rtspnew              SRC_FILES)
aux_source_directory(${BUILD_ROOT}/modules/factory_test/src/          SRC_FILES)
aux_source_directory(${BUILD_ROOT}/hal/wifi/src                       SRC_FILES)
aux_source_directory(${BUILD_ROOT}/hal/led/src                        SRC_FILES)
aux_source_directory(${BUILD_ROOT}/hal/misc/src                       SRC_FILES)
aux_source_directory(${BUILD_ROOT}/hal/motor/src                      SRC_FILES)
aux_source_directory(${BUILD_ROOT}/hal/reset_key/src                  SRC_FILES)
aux_source_directory(${BUILD_ROOT}/hal/i2c/src                        SRC_FILES)
aux_source_directory(${BUILD_ROOT}/main                               SRC_FILES)
aux_source_directory(${BUILD_ROOT}/main/lux_server/src                SRC_FILES)
aux_source_directory(${BUILD_ROOT}/modules/thrdlibs/iniparser-master/iniparser-master/src           SRC_FILES)
aux_source_directory(${BUILD_ROOT}/modules/thrdlibs/rtsp2/src              SRC_FILES)
aux_source_directory(${BUILD_ROOT}/modules/onvif/src_soap_new_2/function_module            SRC_FILES)


#if (CONFIG_JIO_DEMO)
aux_source_directory(${BUILD_ROOT}/main/jio_demo/src  SRC_FILES)
aux_source_directory(${BUILD_ROOT}/modules/jio/src   SRC_FILES)
#else(CONFIG_RTSP)
aux_source_directory(${BUILD_ROOT}/main/rtsp_demo/src SRC_FILES)
#endif()

#onvif
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src_soap_new_2/soapC.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src_soap_new_2/soapServer.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src_soap_new_2/stdsoap2.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src_soap_new_2/dom.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src_soap_new_2/wsseapi.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src_soap_new_2/wsaapi.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src_soap_new_2/threads.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src_soap_new_2/smdevp.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src_soap_new_2/mecevp.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src_soap_new_2/struct_timeval.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src_soap_new_2/httpda.c)
# list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src/onvif_func_2.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src/func_network.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src/onvif.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src/func_log.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src/func_video.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src/func_osd.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/onvif/src/authenticate.c)


# jzdl iaac dl 
list(APPEND TARGET_LIB  pthread m rt stdc++ dl
${JIO_LIB_DIR}/libidip.a 
${JIO_LIB_DIR}/libmsgpackc.a 
${JIO_LIB_DIR}/libwebsockets.a 
${SDK_LIB_DIR}/libalog.a 
${SDK_LIB_DIR}/libimp.a 
#${SDK_LIB_DIR}/libmove_inf.a 
${SDK_LIB_DIR}/libsysutils.a 
${THIRD_LIB_DIR}/libzbar.a 
#${THIRD_LIB_DIR}/libiniparser.a 
${BUILD_ROOT}/libs/7.2.0_1.2.0/fdk-aac/libfdk-aac.a 
#${JIO_LIB_DIR}/libmbedtls.a 
#${JIO_LIB_DIR}/libmbedx509.a 
#${JIO_LIB_DIR}/libmbedcrypto.a 
${BUILD_ROOT}/libs/7.2.0_1.2.0/openssl/libssl.a
${BUILD_ROOT}/libs/7.2.0_1.2.0/openssl/libcrypto.a
#${ALG_COMMON_LIB_DIR}/libmxu_objdetect.a 
#${ALG_COMMON_LIB_DIR}/libfftw3f.a
${ALG_LIB_DIR}/libperm_inf.so 
${ALG_LIB_DIR}/libpersonDet_inf.so
${ALG_LIB_DIR}/libfaceDet_inf.so
${ALG_LIB_DIR}/libvenus.so
${ALG_LIB_DIR}/libmxu_merge.a 
${ALG_LIB_DIR}/libmxu_contrib.a 
${ALG_LIB_DIR}/libmxu_video.a 
${ALG_LIB_DIR}/libmxu_imgproc.a 
${ALG_LIB_DIR}/libmxu_core.a 
${BUILD_ROOT}/libs/7.2.0_1.2.0/opencv/libopencv_core.so.3.3
${BUILD_ROOT}/libs/7.2.0_1.2.0/opencv/libopencv_highgui.so.3.3
${BUILD_ROOT}/libs/7.2.0_1.2.0/opencv/libopencv_imgproc.so.3.3
${BUILD_ROOT}/libs/7.2.0_1.2.0/opencv/libopencv_imgcodecs.so.3.3
${BUILD_ROOT}/libs/7.2.0_1.2.0/opencv/libopencv_videoio.so.3.3
${BUILD_ROOT}/libs/7.2.0_1.2.0/opencv/libopencv_features2d.so.3.3
${BUILD_ROOT}/libs/7.2.0_1.2.0/opencv/libopencv_flann.so.3.3
${BUILD_ROOT}/libs/7.2.0_1.2.0/opencv/libopencv_video.so.3.3
${BUILD_ROOT}/libs/7.2.0_1.2.0/lux/lib_T40_LineCrossDet_opencv_glibc_v230412.so
${BUILD_ROOT}/libs/7.2.0_1.2.0/lux/lib_T40_TamperingDet_opencv_glibc_v230516.so
${BUILD_ROOT}/libs/7.2.0_1.2.0/lux/lib_T40_UMDet_opencv_glibc_v230601.so
${BUILD_ROOT}/libs/7.2.0_1.2.0/lux/lib_T40_LoiterDet_alg_glibc_v230606.so
)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

add_executable(${BIN_NAME}  ${SRC_FILES})

#set_target_properties(${BIN_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,/system/bin")


# ADD_EXECUTABLE(${DEMO_NAME} ${DEME_FILES})

target_link_libraries(${BIN_NAME} ${TARGET_LIB})

# 生成完以后 执行拷贝命令 copybin.sh
# copybin.sh 文件需自建，与 CMakeLists.txt同一目录， 注意不要GIT上传
# 文件示例如下：
##!/bin/bash
#set +v
#mkdir -p /home/humy/samba/bin
#cp /mnt/hgfs/LX_work/IPC/project/test/camaraX_T40/camera_cdb/build/media_sample /home/humy/samba/bin
#chmod 777 /home/humy/samba/bin/media_sample
#set -v

SET(MY_COPY_FILE "${CMAKE_BINARY_DIR}/mycopy.sh")
file(WRITE ${MY_COPY_FILE} "#!/bin/bash\n")
file(APPEND ${MY_COPY_FILE} "set +v\n")
file(APPEND ${MY_COPY_FILE} "if [ -f ${CMAKE_CURRENT_SOURCE_DIR}/copybin.sh ]; then\n")
file(APPEND ${MY_COPY_FILE} "    chmod 777 ${CMAKE_CURRENT_SOURCE_DIR}/copybin.sh\n")
file(APPEND ${MY_COPY_FILE} "    ${CMAKE_CURRENT_SOURCE_DIR}/copybin.sh\n")
file(APPEND ${MY_COPY_FILE} "fi\n")
file(APPEND ${MY_COPY_FILE} "set -v\n")


add_custom_target(run_copy_command 
                  ALL 
                  COMMAND "./mycopy.sh" 
                  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
                  DEPENDS ${BIN_NAME})

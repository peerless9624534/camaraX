cmake_minimum_required(VERSION 3.10.0)

project(onvif)

message(STATUS "current CMAKE_VERSION=${CMAKE_VERSION}")

#指定 C++ std11
set(CMAKE_CXX_STANDARD 11)

#指定项目名称
SET(PROJECT_NAME "onvif")

#指定项目的输出名称
SET(BIN_NAME "onvif")

#设置解决方案工作路径
set (SOLUTION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../)

#设置解决方案名称
if (NOT SOLUTION_NAME)
    set (SOLUTION_NAME ${PROJECT_NAME})
endif()

add_compile_definitions(DISCOVERY_ONLY)

#设置工程工作路径
set (PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

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
SET(CMAKE_C_FLAGS_RELEASE "$ENV{CXXFLAGS} -Wall -O2 -march=mips32r2") 
SET(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -O2 -Wall -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable -march=mips32r2 -s")

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wall -Wl,--gc-section ")

SET(BUILD_ROOT           ${CMAKE_CURRENT_SOURCE_DIR}/..)


include_directories(${BUILD_ROOT}/)
include_directories(${BUILD_ROOT}/common/include/)
include_directories(${BUILD_ROOT}/modules/media/include/)
include_directories(${BUILD_ROOT}/modules/thrdlibs/iniparse/include/)
include_directories(${BUILD_ROOT}/modules/ingenic/1.2.0/imp/include/)
include_directories(${BUILD_ROOT}/onvif_discovery/include/)
include_directories(${BUILD_ROOT}/onvif_discovery/src)
include_directories(${BUILD_ROOT}/modules/openssl/include)              
include_directories(${BUILD_ROOT}/modules/config/include)
include_directories(${BUILD_ROOT}/hal/led/include)



SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 
SET(CMAKE_INSTALL_RPATH "/system/bin")


#new onvif
list(APPEND  SRC_FILES  ${BUILD_ROOT}/onvif_discovery/src/soapC.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/onvif_discovery/src/soapServer.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/onvif_discovery/src/stdsoap2.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/onvif_discovery/src/dom.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/onvif_discovery/src/onvif_func.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/onvif_discovery/src/func_network.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/onvif_discovery/src/func_log.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/modules/media/src/lux_global.c)
list(APPEND  SRC_FILES  ${BUILD_ROOT}/onvif_discovery/src/onvif.c)


list(APPEND TARGET_LIB  pthread m rt stdc++ dl
${BUILD_ROOT}/libs/7.2.0_1.2.0/openssl/libssl.a
${BUILD_ROOT}/libs/7.2.0_1.2.0/openssl/libcrypto.a
)

add_executable(${BIN_NAME}  ${SRC_FILES})

target_link_libraries(${BIN_NAME} ${TARGET_LIB})
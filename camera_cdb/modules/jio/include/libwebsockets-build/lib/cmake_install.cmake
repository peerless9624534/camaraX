# Install script for directory: /home/mc/LuxWorkspace/workspace1/201/ivideon/ivideon-embedded-sdk/thirdparty/libwebsockets/lib

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/mc/LuxWorkspace/workspace1/201/ivideon/ivideon-embedded-sdk/build_jz/libidip/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/home/mc/Toolchain/mips-gcc472-glibc216-64bit/bin/mips-linux-gnu-objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "core" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/mc/LuxWorkspace/workspace1/201/ivideon/ivideon-embedded-sdk/build_jz/libidip/thirdparty/src/libwebsockets-build/lib/libwebsockets.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/mc/LuxWorkspace/workspace1/201/ivideon/ivideon-embedded-sdk/build_jz/libidip/thirdparty/src/libwebsockets-build/libwebsockets.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/mc/LuxWorkspace/workspace1/201/ivideon/ivideon-embedded-sdk/build_jz/libidip/thirdparty/src/libwebsockets-build/libwebsockets_static.pc")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/mc/LuxWorkspace/workspace1/201/ivideon/ivideon-embedded-sdk/build_jz/libidip/thirdparty/src/libwebsockets-build/lib/plat/unix/cmake_install.cmake")
  include("/home/mc/LuxWorkspace/workspace1/201/ivideon/ivideon-embedded-sdk/build_jz/libidip/thirdparty/src/libwebsockets-build/lib/tls/cmake_install.cmake")
  include("/home/mc/LuxWorkspace/workspace1/201/ivideon/ivideon-embedded-sdk/build_jz/libidip/thirdparty/src/libwebsockets-build/lib/core/cmake_install.cmake")
  include("/home/mc/LuxWorkspace/workspace1/201/ivideon/ivideon-embedded-sdk/build_jz/libidip/thirdparty/src/libwebsockets-build/lib/misc/cmake_install.cmake")
  include("/home/mc/LuxWorkspace/workspace1/201/ivideon/ivideon-embedded-sdk/build_jz/libidip/thirdparty/src/libwebsockets-build/lib/system/cmake_install.cmake")
  include("/home/mc/LuxWorkspace/workspace1/201/ivideon/ivideon-embedded-sdk/build_jz/libidip/thirdparty/src/libwebsockets-build/lib/core-net/cmake_install.cmake")
  include("/home/mc/LuxWorkspace/workspace1/201/ivideon/ivideon-embedded-sdk/build_jz/libidip/thirdparty/src/libwebsockets-build/lib/roles/cmake_install.cmake")
  include("/home/mc/LuxWorkspace/workspace1/201/ivideon/ivideon-embedded-sdk/build_jz/libidip/thirdparty/src/libwebsockets-build/lib/event-libs/cmake_install.cmake")

endif()


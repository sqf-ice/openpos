#SET(CMAKE_C_COMPILER "/usr/local/arm-linux/usr/bin/arm-linux-gcc")

PROJECT(OSINI)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -fPIC -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -fPIC -Wall")
SET(CMAKE_INSTALL_PREFIX "../release")
ADD_SUBDIRECTORY( src lib )

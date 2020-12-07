cmake_minimum_required(VERSION 3.13)
project(ParamHelper)

set(CMAKE_CXX_STANDARD 14)

add_library(paramhelper STATIC
        src/filesystem.cpp
        src/params.cpp)


# CMake instructions to test using the static lib

SET( APP_EXE StaticTest )

ADD_EXECUTABLE( ${APP_EXE}
        src/main.cpp )

TARGET_LINK_LIBRARIES( ${APP_EXE}
        paramhelper)


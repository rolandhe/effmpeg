cmake_minimum_required(VERSION 3.22)
project(effmpeg C)


aux_source_directory(./ffmpegg FFM_SRCS)

SET(CMAKE_C_STANDARD 17)
SET (CMAKE_C_COMPILER "gcc")
SET (CMAKE_CXX_COMPILER "g++")
include_directories(/usr/local/include)
link_directories(/usr/local/lib)


add_executable(effmpeg main.c ${FFM_SRCS} ffmpegg/hw.h ffmpegg/hw.c)
target_link_libraries(effmpeg avformat avdevice avcodec avutil avfilter swscale swresample m)

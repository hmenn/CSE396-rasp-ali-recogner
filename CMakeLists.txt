cmake_minimum_required(VERSION 3.2)

project(CSE396_rasp_ali_recogner)
set(CMAKE_CXX_STANDARD 11)
##set(SOURCE_FILES main.cpp)

#set(INCLUDE ./include/Communication.h ./include/ProccesImage.h ./include/rs232.h)
#set(SRC ./src/Communication.cpp ./src/ProccesImage.cpp ./src/rs232.c)

file(GLOB SOURCE_FILE main.cpp "./include/*.h"  "./src/*.c*")

find_package(Threads)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable( CSE396_rasp_ali_recogner main.cpp ${SOURCE_FILE})

target_link_libraries(CSE396_rasp_ali_recogner ${OpenCV_LIBS})
target_link_libraries (CSE396_rasp_ali_recogner ${CMAKE_THREAD_LIBS_INIT})


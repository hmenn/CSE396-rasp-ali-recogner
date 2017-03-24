cmake_minimum_required(VERSION 3.6)
project(CSE396_rasp_ali_recogner)

set(CMAKE_CXX_STANDARD 11)

# opencv libraries
find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})


set(SOURCE_FILES main.cpp)
add_executable(CSE396_rasp_ali_recogner ${SOURCE_FILES})
target_link_libraries( CSE396_rasp_ali_recogner ${OpenCV_LIBS} )
#TODO kendi cmake versiyonunuza gore set edebilirsiniz.
cmake_minimum_required(VERSION 3.6)

project(CSE396_rasp_ali_recogner)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
set(CMAKE_CXX_STANDARD 11)

#TODO daha sonra bunu kullababiliriz.
#file(GLOB SRCS include/*.h src/*.cpp )

set(SOURCE_FILES main.cpp src/ProccesImage.cpp src/Communication.cpp include/Communication.h)

add_executable(FindStickman ${SOURCE_FILES})

target_link_libraries(FindStickman ${OpenCV_LIBS})

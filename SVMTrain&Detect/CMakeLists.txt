cmake_minimum_required(VERSION 2.8)
project( SvmTrain&Detect )
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ../.)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

find_package( OpenCV REQUIRED )


add_executable(train train.cpp)
add_executable(detect detect.cpp)
add_executable(collect collectData.cpp)

target_link_libraries( train ${OpenCV_LIBS}  )
target_link_libraries( detect ${OpenCV_LIBS}  )
target_link_libraries( collect ${OpenCV_LIBS}  )
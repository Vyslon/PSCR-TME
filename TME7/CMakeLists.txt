cmake_minimum_required(VERSION 3.24)
project(TME7)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(TME7
        src/chat_common.h
        src/main.cpp
        src/prod_cons.cpp
        src/Stack.h src/main_main.cpp src/main_prod.cpp src/main_cons.cpp)

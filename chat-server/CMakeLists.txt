cmake_minimum_required(VERSION 3.10.2)
project(chat-server)

set(CMAKE_CXX_STANDARD 17)

set(TARGET_NAME chat-server)
add_executable(${TARGET_NAME} main.cpp vusocket.h vusocket.cpp CircularLineBuffer.cpp CircularLineBuffer.h Server.cpp Server.h Client.cpp Client.h helperFunctions.h)
if(WIN32)
    target_link_libraries(${TARGET_NAME} Ws2_32)
else()
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(${TARGET_NAME} Threads::Threads)
endif()


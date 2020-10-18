cmake_minimum_required(VERSION 3.17)
project(SocketSys)

find_package(GTest)

set(CMAKE_CXX_STANDARD 17)

add_library(SocketSys include/socketsys/socketsys.hpp include/socketsys/provider.hpp src/windows.cpp include/socketsys/platform.hpp include/socketsys/exception.hpp)

target_include_directories(SocketSys PUBLIC include)

if (GTest_FOUND)
    add_subdirectory(tests)
endif()

if(WIN32)
    target_link_libraries(SocketSys PUBLIC Ws2_32)
endif()
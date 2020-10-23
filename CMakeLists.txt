cmake_minimum_required(VERSION 3.17)
project(SocketSys)

find_package(GTest)

set(CMAKE_CXX_STANDARD 17)

add_library(SocketSys
        include/socketsys/socket.hpp
        include/socketsys/provider.hpp
        include/socketsys/platform.hpp
        include/socketsys/exception.hpp
        src/windows/server.cpp
        include/socketsys/config.hpp
        src/windows/client.cpp
        )

target_include_directories(SocketSys PUBLIC include)

if (GTest_FOUND)
    add_subdirectory(tests)
endif()

if(WIN32)
    target_link_libraries(SocketSys PUBLIC Ws2_32)
endif()
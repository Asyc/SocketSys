cmake_minimum_required(VERSION 3.17)
project(SocketSys)

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

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest)
    add_subdirectory(libs/gtest)
else()
    find_package(GTest)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest" OR GTest_FOUND)
    add_subdirectory(tests)
endif()

if(WIN32)
    target_link_libraries(SocketSys PUBLIC Ws2_32)
endif()
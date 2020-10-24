cmake_minimum_required(VERSION 3.16)
project(SocketSys)

set(CMAKE_CXX_STANDARD 17)

set(HEADERS
        include/socketsys/socket.hpp
        include/socketsys/provider.hpp
        include/socketsys/platform.hpp
        include/socketsys/exception.hpp
        include/socketsys/config.hpp
        )


if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest")
    add_subdirectory(libs/gtest)
else()
    find_package(GTest)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest" OR GTest_FOUND)
    add_subdirectory(tests)
endif()

if(WIN32)
    set(SOURCES src/windows/server.cpp src/windows/client.cpp)
    set(OS_LIB Ws2_32)
elseif(UNIX)
    set(SOURCES src/linux/server.cpp src/linux/client.cpp)
endif()

add_library(SocketSys ${HEADERS} src/lib.cpp ${SOURCES})
target_include_directories(SocketSys PUBLIC include)
target_link_libraries(SocketSys PUBLIC ${OS_LIB})
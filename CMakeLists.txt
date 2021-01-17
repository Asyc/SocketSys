cmake_minimum_required(VERSION 3.16)
project(SocketSys)

set(CMAKE_CXX_STANDARD 17)

set(HEADERS
        include/socketsys/socket.hpp
        include/socketsys/platform.hpp
        include/socketsys/exception.hpp
        include/socketsys/internal/windows.hpp
        include/socketsys/config.hpp)


if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest")
    add_subdirectory(libs/gtest)
else()
    find_package(GTest)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest" OR GTest_FOUND)
    add_subdirectory(tests)
endif()

if(WIN32)
    set(SOURCES src/windows/socket.cpp)
    set(OS_LIB Ws2_32)
elseif(UNIX)
    set(SOURCES src/unix/server.cpp src/unix/client.cpp include/socketsys/socket.hpp src/test.hpp src/windows/socket.cpp include/socketsys/address.hpp include/socketsys/internal/windows.hpp)
endif()

add_library(SocketSys ${HEADERS} src/lib.cpp ${SOURCES})
target_include_directories(SocketSys PUBLIC include)
target_link_libraries(SocketSys PUBLIC ${OS_LIB})
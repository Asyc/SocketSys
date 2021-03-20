cmake_minimum_required(VERSION 3.19)
project(SocketSys)

set(CMAKE_CXX_STANDARD 17)

add_library(SocketSys src/library.cpp include/socketsys/tcp/socket.hpp include/socketsys/socketsys.hpp include/socketsys/address/address.hpp include/socketsys/address/address_resolver.hpp src/ipv4.cpp windows/address_resolver.cpp windows/winsock.hpp include/socketsys/error/exception.hpp include/socketsys/address/ipv4.hpp include/socketsys/address/ipv6.hpp src/ipv6.cpp include/socketsys/os/tcp/winsock_tcp.hpp include/socketsys/tcp/settings.hpp)

target_include_directories(SocketSys PUBLIC include)
target_include_directories(SocketSys PRIVATE src)

target_link_libraries(SocketSys PRIVATE Ws2_32)

file(GLOB TEST_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendors/gtest")
list(LENGTH TEST_LIB_DIR TEST_LIB)
if(NOT TEST_LIB EQUAL 0)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendors/gtest")
    set(TEST_ENABLED TRUE)
else()
    find_package(GTest)
endif()

if (TEST_ENABLED OR GTest_FOUND)
    add_subdirectory(tests)
endif()
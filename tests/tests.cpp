#include <iostream>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include "socketsys/socket.hpp"

using namespace socketsys;

TEST(Socket, Create) {
    Socket socket;
}

TEST(Server, Create) {
    ServerSocket socket;
}

TEST(Server, Bind) {
    ServerSocket socket;
    socket.bind("0.0.0.0", 80);
}

TEST(Server, Accept) {
    ServerSocket socket;
    socket.bind("0.0.0.0", 80);

}

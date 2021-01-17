#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include "socketsys/socket.hpp"

TEST(SocketTcp, CreateIpv4) {
    socketsys::tcp::Socket socket;
}

TEST(SocketTcp, ConnectIpv4) {
    socketsys::tcp::Socket socket;
    socket.connect("google.com", 80);

    auto options = socket.getSocketOptions();

}
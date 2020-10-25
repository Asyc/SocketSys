#include <thread>

#include <gtest/gtest.h>

#include "socketsys/socket.hpp"

using namespace socketsys;

constexpr uint16_t PORT = 27000;

TEST(Server, Create) {
    ServerSocket socket(AddressFamily::IPV4);
}

TEST(Server, CreateIPV6) {
    ServerSocket socket(AddressFamily::IPV6);
}

TEST(Server, TCP) {
    ServerSocket socket(AddressFamily::IPV4, SocketProtocol::TCP);
}

TEST(Server, UDP) {
    ServerSocket socket(AddressFamily::IPV4, SocketProtocol::UDP);
}

TEST(Server, Bind) {
    ServerSocket socket;
    socket.setSoLinger(true, 0);
    socket.bind("127.0.0.1", PORT);
}

TEST(Server, BindIPV6) {
    ServerSocket socket(AddressFamily::IPV6);
    socket.setSoLinger(true, 0);
    socket.bind("::1", PORT);
}

TEST(Server, Accept) {
    ServerSocket socket;
    socket.setSoLinger(true, 0);
    socket.bind("127.0.0.1", PORT);

    std::thread thread([=](){
        try {
            Socket socket;
            socket.setSoLinger(true, 0);
            socket.connect("127.0.0.1", PORT);
        } catch (const std::exception& ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    socket.accept();
    thread.join();
}
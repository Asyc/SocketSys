#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include "socketsys/socket.hpp"

using namespace socketsys;


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
    constexpr uint32_t PORT = 27100;

    ServerSocket socket;
    socket.bind("127.0.0.1", PORT);
}

TEST(Server, BindIPV6) {
    constexpr uint32_t PORT = 27101;

    ServerSocket socket(AddressFamily::IPV6);
    socket.bind("::1", PORT);
}

TEST(Server, Accept) {
    constexpr uint32_t PORT = 27102;

    ServerSocket socket;
    socket.bind("127.0.0.1", PORT);

    std::thread thread([=](){
        try {
            Socket socket;
            socket.connect("127.0.0.1", PORT);
        } catch (const std::exception& ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    socket.accept();
    thread.join();
}
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include "socketsys/socket.hpp"

using namespace socketsys;

constexpr uint16_t PORT = 27000;

TEST(Socket, Create) {
    Socket socket(AddressFamily::IPV4);
}

TEST(Socket, CreateIPV6) {
    Socket socket(AddressFamily::IPV6);
}

TEST(Socket, TCP) {
    Socket socket(AddressFamily::IPV4, SocketProtocol::TCP);
    socket.setTcpNoDelay(true);
    socket.setSoReuseAddress(false);
    socket.setSoLinger(true, 0);
    socket.setSoReceiveTimeout(100);
    socket.setSoSendTimeout(100);
    socket.setSoSendBufferSize(100);
    socket.setSoReceiveBufferSize(100);
    socket.setSoKeepAlive(true);
}

TEST(Socket, UDP) {
    Socket socket(AddressFamily::IPV4, SocketProtocol::UDP);
    socket.setSoBroadcast(true);
    socket.setSoLinger(true, 0);
}

TEST(Socket, Connect) {
    ServerSocket server(AddressFamily::IPV6);
    server.setSoLinger(true, 0);
    server.bind("::1", PORT);

    std::thread thread([&]() {
        try {
            server.accept();
        } catch (const std::exception &ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    Socket socket(AddressFamily::IPV6);
    socket.setSoLinger(true, 0);
    socket.connect("::1", PORT);
    thread.join();

}

TEST(Socket, ConnectIPV6) {
    ServerSocket server(AddressFamily::IPV6);
    server.setSoLinger(true, 0);
    server.bind("::1", PORT);

    std::thread thread([&]() {
        try {
            server.accept();
        } catch (const std::exception &ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    Socket socket(AddressFamily::IPV6);
    socket.setSoLinger(0, false);
    socket.connect("::1", PORT);
    thread.join();
}


TEST(Socket, Read) {
    const char *data = "Hello, World\n";
    size_t length = strlen(data) + 1;

    ServerSocket server;
    server.setSoLinger(true, 0);
    server.bind("127.0.0.1", PORT);

    std::thread thread([&]() {
        try {
            auto handle = server.accept();
            handle.write(data, length);
        } catch (const std::exception &ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    Socket socket;
    socket.setSoLinger(true, 0);
    socket.connect("127.0.0.1", PORT);

    std::vector<char> buffer(length);
    auto read = socket.read(buffer);
    while (read != length) read += socket.read(buffer);

    thread.join();
    EXPECT_TRUE(strcmp(buffer.data(), data) == 0);
}

TEST(Socket, Write) {
    const char *data = "Hello, World\n";
    size_t length = strlen(data) + 1;

    ServerSocket server;
    server.setSoLinger(true, 0);
    server.bind("0.0.0.0", PORT);

    std::thread thread([&]() {
        try {
            auto handle = server.accept();
            std::vector<char> buffer(length);
            handle.read(buffer);
            EXPECT_TRUE(strcmp(buffer.data(), data) == 0);
        } catch (const std::exception &ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    Socket socket;
    socket.setSoLinger(true, 0);
    socket.connect("127.0.0.1", PORT);

    socket.write(data, length);

    thread.join();
}

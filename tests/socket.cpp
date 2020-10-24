#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include "socketsys/socket.hpp"

using namespace socketsys;

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
    socket.setSoLinger(false, 0);
    socket.setSoReceiveTimeout(100);
    socket.setSoSendTimeout(100);
    socket.setSoSendBufferSize(100);
    socket.setSoReceiveBufferSize(100);
    socket.setSoKeepAlive(true);
}

TEST(Socket, UDP) {
    Socket socket(AddressFamily::IPV4, SocketProtocol::UDP);
    socket.setSoBroadcast(true);
}

TEST(Socket, Connect) {
    constexpr uint32_t PORT = 27000;

    std::thread thread([=](){
        try {
            ServerSocket socket(AddressFamily::IPV6);
            socket.bind("::1", PORT);
            socket.accept();
        } catch (const std::exception& ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    Socket socket(AddressFamily::IPV6);
    socket.connect("::1", PORT);
    thread.join();
}

TEST(Socket, ConnectIPV6) {
    constexpr uint32_t PORT = 27001;

    std::thread thread([=](){
        try {
            ServerSocket socket(AddressFamily::IPV6);
            socket.bind("::1", PORT);
            socket.accept();
        } catch (const std::exception& ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    Socket socket(AddressFamily::IPV6);
    socket.connect("::1", PORT);
    thread.join();
}


TEST(Socket, Read) {
    constexpr uint32_t PORT = 27002;
    const char* data = "Hello, World\n";
    size_t length = strlen(data) + 1;

    std::thread thread([=](){
        try {
            ServerSocket socket;
            socket.bind("127.0.0.1", PORT);
            auto handle = socket.accept();
            handle.write(data, length);
        } catch (const std::exception& ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    Socket socket;
    socket.connect("127.0.0.1", PORT);

    std::vector<char> buffer(length);
    auto read = socket.read(buffer);
    while (read != length) read += socket.read(buffer);

    thread.join();
    EXPECT_TRUE(strcmp(buffer.data(), data) == 0);
}

TEST(Socket, Write) {
    constexpr uint32_t PORT = 27003;
    const char* data = "Hello, World\n";
    size_t length = strlen(data) + 1;

    std::thread thread([=](){
        try {
            ServerSocket socket;
            socket.bind("0.0.0.0", PORT);
            auto handle = socket.accept();

            std::vector<char> buffer(length);
            handle.read(buffer);
            EXPECT_TRUE(strcmp(buffer.data(), data) == 0);
        } catch (const std::exception& ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    Socket socket;
    socket.connect("127.0.0.1", PORT);

    socket.write(data, length);

    thread.join();
}

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
    constexpr uint16_t PORT = 27000;

    ServerSocket server(AddressFamily::IPV4);
    server.setSoLinger(false, 0);
    server.bind("0.0.0.0", PORT);

    std::thread thread([&]() {
        try {
            server.accept();
        } catch (const std::exception& ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    Socket socket(AddressFamily::IPV4);
    socket.connect("127.0.0.1", PORT);
    thread.join();
}

TEST(Socket, ConnectIPV6) {
    constexpr uint16_t PORT = 27000;

    ServerSocket server(AddressFamily::IPV6);
    server.bind("::1", PORT);

    std::thread thread([&]() {
        try {
            server.accept();
        } catch (const std::exception &ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    Socket socket(AddressFamily::IPV6);
    socket.setSoLinger(false, 0);
    socket.connect("::1", PORT);
    thread.join();
}


TEST(Socket, Read) {
    constexpr uint16_t PORT = 27001;

    const char* data = "Hello, World\n";
    size_t length = strlen(data) + 1;

    ServerSocket server;
    server.bind("127.0.0.1", PORT);

    std::thread thread([&]() {
        try {
            auto handle = server.accept();
            handle.write(data, length);
        } catch (const std::exception &ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    std::vector<char> buffer(length);

    {
        Socket socket;
        socket.setSoLinger(false, 0);
        socket.connect("127.0.0.1", PORT);

        auto read = socket.read(buffer);
        while (read != length) read += socket.read(buffer);
    }

    thread.join();
    EXPECT_TRUE(strcmp(buffer.data(), data) == 0);
}

TEST(Socket, Write) {
    constexpr uint16_t PORT = 27002;

    const char* data = "Hello, World\n";
    size_t length = strlen(data) + 1;

    ServerSocket server;
    server.bind("127.0.0.1", PORT);

    std::thread thread([&]() {
        try {
            auto handle = server.accept();
            handle.setSoLinger(false, 0);

            std::vector<char> buffer(length);
            size_t read = handle.read(buffer);
            while (read != length) read += handle.read(buffer);

            EXPECT_TRUE(strcmp(buffer.data(), data) == 0);
        } catch (const std::exception& ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    {
        Socket socket;
        socket.setSoLinger(false, 0);
        socket.connect("127.0.0.1", PORT);
        socket.write(data, length);
    }

    thread.join();
}

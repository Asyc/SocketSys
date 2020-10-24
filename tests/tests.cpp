#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include "socketsys/socket.hpp"

using namespace socketsys;

TEST(Socket, Create) {
    Socket();
}

TEST(Socket, Connect) {
    Socket socket;

    std::thread thread([](){
        try {
            ServerSocket socket;
            socket.bind("127.0.0.1", 27000);
            socket.accept();
        } catch (const std::exception& ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    socket.connect("127.0.0.1", 27000);
    thread.join();
}

TEST(Socket, IO) {
    const char* sendData = "send";

    ServerSocket socket;
    socket.bind("127.0.0.1", 27001);

    std::thread thread([sendData](){
        try {
            Socket socket;
            socket.connect("127.0.0.1", 27001);
            socket.write(sendData, 5);
        } catch (const std::exception& ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    auto handle = socket.accept();
    std::vector<char> data(5);

    size_t read = handle.read(data.data(), data.size());
    while (read != 5) {
        read += handle.read(data.data() + read, data.size());
    }

    thread.join();

    EXPECT_TRUE(strcmp(data.data(), sendData) == 0);
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

TEST(Server, Create) {
    ServerSocket();
}

TEST(Server, IPV4) {
    ServerSocket socket;
    socket.bind("127.0.0.1", 27002);
}

TEST(Server, IPV6) {
    ServerSocket socket(AddressFamily::IPV6);
    socket.bind("::1", 27003);
}

TEST(Server, Accept) {
    ServerSocket socket;
    socket.bind("127.0.0.1", 27004);

    std::thread thread([]() {
        try {
            Socket socket;
            socket.connect("127.0.0.1", 27004);
        } catch (const std::exception& ex) {
            FAIL() << ex.what();
        }
    });

    socket.accept();
    thread.join();
}
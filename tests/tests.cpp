#include <iostream>
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
            socket.bind("127.0.0.1", 25566);
            socket.accept();
        } catch (const std::exception& ex) {
            FAIL() << ex.what() << '\n';
        }
    });

    socket.connect("127.0.0.1", 25566);
    thread.join();
}

TEST(Socket, IO) {
    const char* sendData = "send";

    ServerSocket socket;
    socket.bind("127.0.0.1", 81);

    std::thread thread([sendData](){
        try {
            Socket socket;
            socket.connect("127.0.0.1", 81);
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

TEST(Server, Create) {
    ServerSocket();
}

TEST(Server, Bind) {
    ServerSocket socket;
    socket.bind("127.0.0.1", 80);
}

TEST(Server, Accept) {
    ServerSocket socket;
    socket.bind("127.0.0.1", 25565);

    std::thread thread([]() {
        try {
            Socket socket;
            socket.connect("127.0.0.1", 25565);
        } catch (const std::exception& ex) {
            FAIL() << ex.what();
        }
    });

    socket.accept();
    thread.join();
}
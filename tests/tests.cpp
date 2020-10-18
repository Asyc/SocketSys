#include <iostream>
#include <vector>

#include <gtest/gtest.h>

#include "socketsys/socketsys.hpp"

using namespace socketsys;

TEST(Client, Create) {
    Socket socket;
}

TEST(Client, Bind) {
    try {
        Socket socket;
        socket.bind("google.com", 80);
    } catch (const std::exception& ex) {
        FAIL() << ex.what() << '\n';
    }
}

TEST(Client, Read) {
    try {
        std::vector<char> buffer(100);

        Socket socket;
        socket.bind("google.com", 80);
        socket.read(buffer.data(), buffer.size());
        std::cout << buffer[0];
    } catch (const std::exception& ex) {
        FAIL() << ex.what() << '\n';
    }
}
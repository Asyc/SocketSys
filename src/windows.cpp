#include <array>
#include <cstdlib>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include "socketsys/exception.hpp"
#include "socketsys/provider.hpp"

using namespace socketsys;

WindowsProvider::SocketHandle WindowsProvider::init() {
    WSAData data{};
    auto status = WSAStartup(MAKEWORD(2, 1), &data);
    if (status != 0) {
        throw InitException(WSAGetLastError());
    }

    return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void WindowsProvider::bind(SocketHandle handle, const std::string_view& address, uint16_t port) {
    std::array<char, 15> portBuffer{};
    sprintf(portBuffer.data(), "%d", port);

    addrinfo hints{
            {},
            AF_INET,
            SOCK_STREAM,
            IPPROTO_TCP
    };

    addrinfo* result;

    auto addressInfoStatus = getaddrinfo("", portBuffer.data(), &hints, &result);

    if (addressInfoStatus != 0) {
        throw NameResolveException(addressInfoStatus);
    }

    auto bindStatus = ::connect(handle, result[0].ai_addr, sizeof(sockaddr_in));

    if (bindStatus != 0) {
        throw BindException(WSAGetLastError());
    }

    freeaddrinfo(result);
}

size_t WindowsProvider::read(SocketHandle handle, char* buffer, size_t length) {
    auto size = recv(handle, buffer, static_cast<int>(length), 0);
    if (size == SOCKET_ERROR || size == 0) {
        throw ReadException(WSAGetLastError());
    }
    return size;
}

void WindowsProvider::destroy(SocketHandle handle) {
    closesocket(handle);
    WSACleanup();
}
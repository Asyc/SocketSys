#include "socketsys/provider.hpp"

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <array>

#include "socketsys/config.hpp"
#include "socketsys/exception.hpp"
#include "socketsys/socket.hpp"

using namespace socketsys;

WinSockServerProvider::SocketHandle WinSockServerProvider::init(AddressFamily addressFamily, SocketProtocol protocol) {
    WSAData data{};
    auto status = WSAStartup(MAKEWORD(2, 2), &data);
    if (status != 0) {
        throw SocketInitException("WinSock failed to initialize", WSAGetLastError());
    }

    int family = addressFamily == AddressFamily::IPV4 ? AF_INET : AF_INET6;
    int type = protocol == SocketProtocol::TCP ? SOCK_STREAM : SOCK_DGRAM;
    int protocolMask = protocol == SocketProtocol::TCP ? IPPROTO_TCP : IPPROTO_UDP;

    return socket(family, type, protocolMask);
}

void WinSockServerProvider::bind(SocketHandle handle, const std::string_view& address, uint16_t port, size_t backlog) {
    std::array<char, 6> portBuffer{};
    sprintf(portBuffer.data(), "%d", port);

    WSAPROTOCOL_INFOW proto;
    WSADuplicateSocketW(handle, GetCurrentProcessId(), &proto);

    addrinfo hints{
            {},
            proto.iAddressFamily,
            proto.iSocketType,
            proto.iProtocol
    };

    addrinfo* info;
    auto result = getaddrinfo(address.data(), portBuffer.data(), &hints, &info);
    if (result != 0) throw NameResolveException("WinSock failed to resolve name ", WSAGetLastError());

    if (::bind(handle, info[0].ai_addr, sizeof(sockaddr)) + listen(handle, backlog) != 0) {
        throw BindException("WinSock failed to bind server socket ", WSAGetLastError());
    }
}

WinSockServerProvider::ClientHandle WinSockServerProvider::accept(SocketHandle handle) {
    auto socket = ::accept(handle, nullptr, nullptr);
    return ClientHandle(socket);
}

void WinSockServerProvider::swap(SocketHandle& lhs, SocketHandle& rhs) {
    lhs = rhs;
    rhs = INVALID_SOCKET;
}

void WinSockServerProvider::destroy(SocketHandle handle) {
    closesocket(handle);
}
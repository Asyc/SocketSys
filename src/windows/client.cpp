#include "socketsys/socket.hpp"

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <array>

#include "socketsys/exception.hpp"

using namespace socketsys;

WinSockProvider::SocketHandle WinSockProvider::init(AddressFamily addressFamily, SocketProtocol protocol) {
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

void WinSockProvider::destroy(SocketHandle handle) {
    if (handle != INVALID_SOCKET) {
        closesocket(handle);
        WSACleanup();
    }
}

void WinSockProvider::swap(SocketHandle& lhs, SocketHandle& rhs) {
    lhs = rhs;
    rhs = INVALID_SOCKET;
}

void WinSockProvider::connect(SocketHandle handle, const std::string_view& address, uint16_t port) {
    std::array<char, 6> portBuffer{};
    sprintf_s(portBuffer.data(), portBuffer.size(), "%d", port);

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

    result = ::connect(handle, info->ai_addr, info->ai_addrlen) == 0;
    freeaddrinfo(info);

    if (result == 0) {
        throw BindException("WinSock failed to bind socket ", WSAGetLastError());
    }
}

size_t WinSockProvider::read(SocketHandle handle, char* buffer, size_t length) {
    return recv(handle, buffer, length, NULL);
}

size_t WinSockProvider::write(SocketHandle handle, const char* buffer, size_t length) {
    auto result = send(handle, buffer, length, NULL);
    if (result == SOCKET_ERROR) {
        throw IOException("WinSock failed to write to socket", WSAGetLastError());
    }

    return length;
}

template <typename Value>
inline void setsockopt(WinSockProvider::SocketHandle handle, int level, int pName, Value value) {
    auto status = ::setsockopt(handle, level, pName, reinterpret_cast<char*>(&value), sizeof(Value));
    if (status != 0) {
        throw SocketOptionError("WinSock failed to set socket option ", WSAGetLastError());
    }
}

void WinSockProvider::setTcpNoDelay(SocketHandle handle, bool flag) {
    setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, static_cast<BOOL>(flag));
}

void WinSockProvider::setSoReuseAddress(SocketHandle handle, bool flag) {
    setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, static_cast<BOOL>(flag));
}

void WinSockProvider::setSoBroadcast(SocketHandle handle, bool flag) {
    setsockopt(handle, SOL_SOCKET, SO_BROADCAST, static_cast<BOOL>(flag));
}

void WinSockProvider::setSoLinger(SocketHandle handle, bool flag, u_short linger) {
    using Linger = ::linger;
    Linger lingerData{
        static_cast<u_short>(flag),
        linger
    };

    setsockopt(handle, SOL_SOCKET, SO_LINGER, lingerData);
}

void WinSockProvider::setSoReceiveTimeout(SocketHandle handle, uint32_t timeout) {
    setsockopt(handle, SOL_SOCKET, SO_RCVTIMEO, timeout);
}

void WinSockProvider::setSoSendTimeout(SocketHandle handle, uint32_t timeout) {
    setsockopt(handle, SOL_SOCKET, SO_SNDTIMEO, timeout);
}

void WinSockProvider::setSoSendBufferSize(SocketHandle handle, int size) {
    setsockopt(handle, SOL_SOCKET, SO_RCVBUF, size);
}

void WinSockProvider::setSoReceiveBufferSize(SocketHandle handle, int size) {
    setsockopt(handle, SOL_SOCKET, SO_SNDBUF, size);
}

void WinSockProvider::setSoKeepAlive(SocketHandle handle, bool flag) {
    setsockopt(handle, SOL_SOCKET, SO_KEEPALIVE, static_cast<BOOL>(flag));
}

void WinSockProvider::setSoInlineOOB(SocketHandle handle, bool flag) {
    setsockopt(handle, SOL_SOCKET, SO_OOBINLINE, static_cast<BOOL>(flag));
}
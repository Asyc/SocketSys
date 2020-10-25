#include "socketsys/provider.hpp"

#include <cerrno>
#include <netdb.h>
#include <netinet/tcp.h>
#include <unistd.h>

#include <array>

#include "socketsys/exception.hpp"

using namespace socketsys;

UnixProvider::SocketHandle UnixProvider::init(AddressFamily addressFamily, SocketProtocol protocol) {
    int family = addressFamily == AddressFamily::IPV4 ? AF_INET : AF_INET6;
    int type = protocol == SocketProtocol::TCP ? SOCK_STREAM : SOCK_DGRAM;
    int protocolMask = protocol == SocketProtocol::TCP ? IPPROTO_TCP : IPPROTO_UDP;

    auto result = socket(family, type, protocolMask);

    if (result == -1) throw SocketInitException("Linux Failed to Create Socket ", errno);

    return result;
}

void UnixProvider::destroy(SocketHandle handle) {
    if (handle != -1) {
        close(handle);
    }
}

void UnixProvider::swap(SocketHandle& lhs, SocketHandle& rhs) {
    lhs = rhs;
    rhs = -1;
}

void UnixProvider::connect(SocketHandle handle, const std::string_view& address, uint16_t port) {
    std::array<char, 6> portBuffer{};
    sprintf(portBuffer.data(), "%d", port);

    sockaddr data{};
    socklen_t size = sizeof(sockaddr);
    getsockname(handle, &data, &size);

    int type;
    socklen_t length = sizeof(int);
    getsockopt(handle, SOL_SOCKET, SO_TYPE, &type, &length);

    addrinfo hints {
            {},
            data.sa_family,
            type,
            type == SOCK_STREAM ? IPPROTO_TCP : IPPROTO_UDP,
    };

    addrinfo* res;
    auto result = getaddrinfo(address.data(), portBuffer.data(), &hints, &res);
    if (result != 0) {
        throw NameResolveException("Linux failed to resolve address ", result);
    }

    result = ::connect(handle, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);

    if (result == -1) {
        throw SocketBindException("Linux failed to connect socket ", errno);
    }
}

size_t UnixProvider::read(SocketHandle handle, char* buffer, size_t length) {
    auto result = recv(handle, buffer, length, 0);
    if (result == -1) {
        throw IOException("Linux failed to read from socket ", errno);
    }
    
    return result;
}

size_t UnixProvider::write(SocketHandle handle, const char* buffer, size_t length) {
    auto result = send(handle, buffer, length, 0);
    if (result == -1) {
        throw IOException("Linux failed to write to socket ", errno);
    }
    
    return result;
}

template <typename Value>
inline void setsockopt(UnixProvider::SocketHandle handle, int level, int pName, Value value) {
    auto status = ::setsockopt(handle, level, pName, reinterpret_cast<char*>(&value), sizeof(Value));
    if (status == -1) {
        throw SocketOptionError("Linux failed to set socket option ", errno);
    }
}

void UnixProvider::setTcpNoDelay(SocketHandle handle, bool flag) {
#ifndef SOL_TCP
#define SOL_TCP IPPROTO_TCP
#endif
    setsockopt(handle, SOL_TCP, TCP_NODELAY, static_cast<int>(flag));
}

void UnixProvider::setSoReuseAddress(SocketHandle handle, bool flag) {
    setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, static_cast<int>(flag));
}

void UnixProvider::setSoBroadcast(SocketHandle handle, bool flag) {
    setsockopt(handle, SOL_SOCKET, SO_BROADCAST, static_cast<int>(flag));
}

void UnixProvider::setSoLinger(SocketHandle handle, bool flag, u_short linger) {
    using Linger = ::linger;
    Linger lingerData{
            static_cast<u_short>(flag),
            linger
    };

    setsockopt(handle, SOL_SOCKET, SO_LINGER, lingerData);
}

void UnixProvider::setSoReceiveTimeout(SocketHandle handle, uint32_t timeout) {
    timeval time{
            timeout,
            0
    };

    setsockopt(handle, SOL_SOCKET, SO_RCVTIMEO, time);
}

void UnixProvider::setSoSendTimeout(SocketHandle handle, uint32_t timeout) {
    timeval time{
        timeout,
        0
    };
    setsockopt(handle, SOL_SOCKET, SO_SNDTIMEO, time);
}

void UnixProvider::setSoSendBufferSize(SocketHandle handle, int size) {
    setsockopt(handle, SOL_SOCKET, SO_RCVBUF, size);
}

void UnixProvider::setSoReceiveBufferSize(SocketHandle handle, int size) {
    setsockopt(handle, SOL_SOCKET, SO_SNDBUF, size);
}

void UnixProvider::setSoKeepAlive(SocketHandle handle, bool flag) {
    setsockopt(handle, SOL_SOCKET, SO_KEEPALIVE, static_cast<int>(flag));
}

void UnixProvider::setSoInlineOOB(SocketHandle handle, bool flag) {
    setsockopt(handle, SOL_SOCKET, SO_OOBINLINE, static_cast<int>(flag));
}
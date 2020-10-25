#include "socketsys/provider.hpp"

#include <cerrno>
#include <netdb.h>
#include <unistd.h>

#include <array>

#include "socketsys/exception.hpp"
#include "socketsys/socket.hpp"

using namespace socketsys;

UnixServerProvider::SocketHandle UnixServerProvider::init(AddressFamily addressFamily, SocketProtocol protocol) {
    int family = addressFamily == AddressFamily::IPV4 ? AF_INET : AF_INET6;
    int type = protocol == SocketProtocol::TCP ? SOCK_STREAM : SOCK_DGRAM;
    int protocolMask = protocol == SocketProtocol::TCP ? IPPROTO_TCP : IPPROTO_UDP;

    return socket(family, type, protocolMask);
}

void UnixServerProvider::swap(SocketHandle& lhs, SocketHandle& rhs) {
    lhs = rhs;
    rhs = -1;
}

void UnixServerProvider::destroy(SocketHandle handle) {
    if (handle != -1) {
        close(handle);
    }
}

void UnixServerProvider::bind(SocketHandle handle, const std::string_view& address, uint16_t port, size_t backlog) {
    std::array<char, 6> portBuffer{};
    sprintf(portBuffer.data(),"%d", port);

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

    result = ::bind(handle, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);

    if (result != -1) result = listen(handle, static_cast<int>(backlog));
    if (result == -1) {
        throw SocketBindException("Linux failed to bind server socket ", errno);
    }
}

UnixServerProvider::ClientHandle UnixServerProvider::accept(SocketHandle handle) {
    auto socket = ::accept(handle, nullptr, nullptr);
    return ClientHandle(socket);
}

void UnixServerProvider::setSoReuseAddress(SocketHandle handle, bool flag) {
    int bFlag = flag;
    setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&bFlag), sizeof(int));
}

void UnixServerProvider::setSoLinger(SocketHandle handle, bool flag, uint16_t seconds) {
    linger linger{
        flag, seconds
    };

    setsockopt(handle, SOL_SOCKET, SO_LINGER, reinterpret_cast<const char*>(&linger), sizeof(linger));
}
#include "socketsys/socket.hpp"

#include <WinSock2.h>
#include <WS2tcpip.h>

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
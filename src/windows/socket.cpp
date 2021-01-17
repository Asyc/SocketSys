#include <WinSock2.h>
#include <WS2tcpip.h>

#include <array>
#include <memory>
#include <vector>
#include <socketsys/internal/windows.hpp>

#include "socketsys/address.hpp"
#include "socketsys/config.hpp"
#include "socketsys/platform.hpp"
#include "socketsys/exception.hpp"

namespace socketsys::tcp {

uint64_t WinSockProvider::newSocket(AddressFamily family) {
    static_assert(std::is_same<SOCKET, Handle>::value);

    WSAData data; // NOLINT
    auto startupError = WSAStartup(MAKEWORD(2, 2), &data);
    if (startupError) throw SocketCreateError(startupError);

    int addressFamily;
    switch (family) {
        case AddressFamily::IPV4:
            addressFamily = AF_INET;
            break;
        case AddressFamily::IPV6:
            addressFamily = AF_INET6;
            break;
        default:
            throw std::runtime_error("unknown address family");
    }

    auto handle = socket(addressFamily, SOCK_STREAM, IPPROTO_TCP);
    if (handle != INVALID_SOCKET) return handle;

    auto error = WSAGetLastError();
    switch (error) {
        case WSAEAFNOSUPPORT: throw AddressFamilyUnsupported(error);
        case WSAEMFILE:
        case WSAENOBUFS: throw MaximumSocketsError(error);
        case WSAEPROTONOSUPPORT: throw UnsupportedProtocolError(error);
        default: throw SocketCreateError(error);
    }
}

void WinSockProvider::destroy(Handle handle) {
    closesocket(handle);
    WSACleanup();
}

void WinSockProvider::connect(Handle handle, const IpAddress& remoteAddress, uint16_t remotePort) {
    std::array<char, 6> portBuffer{};
    sprintf_s(portBuffer.data(), portBuffer.size(), "%d", remotePort);

    WSAPROTOCOL_INFOW protocolInfo;
    int size = sizeof(protocolInfo);
    auto error = getsockopt(handle, SOL_SOCKET, SO_PROTOCOL_INFOW, reinterpret_cast<char*>(&protocolInfo), &size);
    if (error == SOCKET_ERROR) {
        throw SocketOperationError(WSAGetLastError());
    }


    addrinfo hints{};
    hints.ai_family = protocolInfo.iAddressFamily;
    hints.ai_socktype = protocolInfo.iSocketType;
    hints.ai_protocol = protocolInfo.iProtocol;

    addrinfo* addressInfo;
    error = getaddrinfo(remoteAddress.data(), portBuffer.data(), &hints, &addressInfo);
    if (error != 0) throw AddressUnresolvableError(WSAGetLastError());
    error = ::connect(handle, addressInfo->ai_addr, sizeof(sockaddr));
    freeaddrinfo(addressInfo);
    if (error != SOCKET_ERROR) return;
    error = WSAGetLastError();
    switch (error) {
        case WSAENETDOWN:
        case WSAENETUNREACH:
            throw NetworkDownError(error);
        case WSAEADDRINUSE: throw AddressInUseError(error);
        case WSAEADDRNOTAVAIL:
        case WSAEAFNOSUPPORT:
        case WSAEFAULT:
            throw AddressUnresolvableError(error);
        case WSAECONNREFUSED: throw ConnectionRefusedError(error);
        case WSAEISCONN: throw SocketAlreadyConnectedError(error);
        case WSAEHOSTUNREACH:
        case WSAETIMEDOUT:
            throw HostUnreachableError(error);
        default:
            throw SocketConnectError(error);
    }
}

template <typename T, int Opt>
inline void setSockOpt(SOCKET handle, T val) {
    setsockopt(handle, SOL_SOCKET, Opt, reinterpret_cast<char*>(&val), sizeof(T));
}

void WinSockProvider::setSocketOptions(Handle handle, const SocketConfig& config) {
    setSockOpt<DWORD, SO_EXCLUSIVEADDRUSE>(handle, config.soExclusiveAddressUse);
    setSockOpt<DWORD, SO_REUSEADDR>(handle, config.soReuseAddress);
    setSockOpt<DWORD, SO_KEEPALIVE>(handle, config.soKeepAlive);
    setSockOpt<DWORD, SO_OOBINLINE>(handle, config.soInlineOOB);

    setSockOpt<DWORD, SO_RCVBUF>(handle, config.soRcvBuf);
    setSockOpt<DWORD, SO_SNDBUF>(handle, config.soSndBuf);
    setSockOpt<DWORD, SO_RCVTIMEO>(handle, config.soRcvTimeout);
    setSockOpt<DWORD, SO_SNDTIMEO>(handle, config.soSndTimeout);

    linger lingerSettings {
        config.soLinger != 0,
        config.soLinger
    };

    setSockOpt<linger, SO_LINGER>(handle, lingerSettings);
}

}   // namespace socketsys::tcp
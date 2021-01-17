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
            throw AddressFamilyUnsupported(-1);
    }

    auto handle = socket(addressFamily, SOCK_STREAM, IPPROTO_TCP);
    if (handle != INVALID_SOCKET) return handle;

    auto error = WSAGetLastError();
    switch (error) {
        case WSAEAFNOSUPPORT: throw AddressFamilyUnsupported(error);
        case WSAEMFILE:
        case WSAENOBUFS: throw MaximumSocketsError(error);
        case WSAEPROTONOSUPPORT: throw UnsupportedProtocolError(error);
        default: throw SocketException("Unknown error while creating socket", error);
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
            throw SocketException("Unknown error while connecting socket", error);
    }
}

template <typename T, int Level, int Opt>
inline void setSockOpt(SOCKET handle, T val) {
    auto error = setsockopt(handle, Level, Opt, reinterpret_cast<char*>(&val), sizeof(T));
    if (error != SOCKET_ERROR) return;
    error = WSAGetLastError();
    switch (error) {
        case WSAENETDOWN: throw NetworkDownError(error);
        case WSAENETRESET:
        case WSAENOTCONN:
            throw ConnectionResetError(error);
        case WSAENOPROTOOPT:
            throw UnsupportedProtocolError(error);
        default:
            throw SocketException("Unknown error while completing socket operation", error);
    }
}

void WinSockProvider::setSocketOptions(Handle handle, const SocketConfig& config) {
    setSockOpt<DWORD, SOL_SOCKET, SO_EXCLUSIVEADDRUSE>(handle, config.soExclusiveAddressUse);
    setSockOpt<DWORD, SOL_SOCKET, SO_REUSEADDR>(handle, config.soReuseAddress);
    setSockOpt<DWORD, SOL_SOCKET, SO_KEEPALIVE>(handle, config.soKeepAlive);
    setSockOpt<DWORD, SOL_SOCKET, SO_OOBINLINE>(handle, config.soInlineOOB);

    setSockOpt<DWORD, SOL_SOCKET, SO_RCVBUF>(handle, config.soRcvBuf);
    setSockOpt<DWORD, SOL_SOCKET, SO_SNDBUF>(handle, config.soSndBuf);
    setSockOpt<DWORD, SOL_SOCKET, SO_RCVTIMEO>(handle, config.soRcvTimeout);
    setSockOpt<DWORD, SOL_SOCKET, SO_SNDTIMEO>(handle, config.soSndTimeout);

    linger lingerSettings {
        config.soLinger != 0,
        config.soLinger
    };

    setSockOpt<linger, SOL_SOCKET, SO_LINGER>(handle, lingerSettings);


}

template <typename T, int Level, int Opt>
inline T getSockOpt(SOCKET handle) {
    T val{};
    int len = sizeof(T);
    auto error = getsockopt(handle, Level, Opt, reinterpret_cast<char*>(&val), &len);
    if (error != SOCKET_ERROR) return val;
    error = WSAGetLastError();
    switch (error) {
        case WSAENETDOWN: throw NetworkDownError(error);
        case WSAENETRESET:
        case WSAENOTCONN:
            throw ConnectionResetError(error);
        case WSAENOPROTOOPT:
            throw UnsupportedProtocolError(error);
        default:
            throw SocketException("Unknown error while completing socket operation", error);
    }
}


void WinSockProvider::getSocketOptions(WinSockProvider::Handle handle, SocketConfig& out) {
    out.soExclusiveAddressUse = getSockOpt<DWORD, SOL_SOCKET, SO_EXCLUSIVEADDRUSE>(handle);
    out.soReuseAddress = getSockOpt<DWORD, SOL_SOCKET, SO_REUSEADDR>(handle);
    out.soKeepAlive = getSockOpt<DWORD, SOL_SOCKET, SO_KEEPALIVE>(handle);
    out.soInlineOOB = getSockOpt<DWORD, SOL_SOCKET, SO_OOBINLINE>(handle);

    out.soRcvBuf = getSockOpt<DWORD, SOL_SOCKET, SO_RCVBUF>(handle);
    out.soSndBuf = getSockOpt<DWORD, SOL_SOCKET, SO_SNDBUF>(handle);
    out.soRcvTimeout = getSockOpt<DWORD, SOL_SOCKET, SO_RCVTIMEO>(handle);
    out.soSndTimeout = getSockOpt<DWORD, SOL_SOCKET, SO_SNDTIMEO>(handle);

    auto lingerSettings = getSockOpt<linger, SOL_SOCKET, SO_LINGER>(handle);
    if (lingerSettings.l_onoff) {
        out.soLinger = lingerSettings.l_linger;
    } else {
        out.soLinger = 0;
    }

    out.tcpKeepAliveCount = getSockOpt<DWORD, IPPROTO_TCP, TCP_KEEPCNT>(handle);
    out.tcpNoDelay = getSockOpt<DWORD, IPPROTO_TCP, TCP_NODELAY>(handle);
    out.tcpTimestamps = getSockOpt<DWORD, IPPROTO_TCP, TCP_TIMESTAMPS>(handle);
    out.tcpKeepAlive = getSockOpt<DWORD, IPPROTO_TCP, TCP_KEEPALIVE>(handle);
    out.tcpKeepAliveInterval = getSockOpt<DWORD, IPPROTO_TCP, TCP_KEEPINTVL>(handle);

}

}   // namespace socketsys::tcp
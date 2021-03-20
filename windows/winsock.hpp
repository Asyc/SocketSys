#ifndef SOCKETSYS_WINDOWS_WINSOCK_HPP_
#define SOCKETSYS_WINDOWS_WINSOCK_HPP_

#include <WinSock2.h>

#include <memory>
#include <string_view>

#include "socketsys/error/exception.hpp"

namespace socketsys::windows {

void mapError(size_t code);

void startup() {
    WSAData data;   //NOLINT

    auto status = WSAStartup(MAKEWORD(2, 1), &data);
    mapError(status);
}

void cleanup() {
    mapError(WSACleanup());
}

void mapError(size_t code) {
    switch (code) {
        case 0:break;
        case WSAEAFNOSUPPORT:throw error::UnsupportedAddressFamilyError(code);
        case WSASYSNOTREADY:throw error::windows::WindowsSystemNotReadyError(code);
        case WSAVERNOTSUPPORTED:throw error::windows::WindowsUnsupportedVersionError(code);
        case WSAEINPROGRESS:throw error::windows::WindowsBlockingActionError(code);
        case WSAEPROCLIM:throw error::windows::WindowsMaxInstanceError(code);
        case WSAHOST_NOT_FOUND:throw error::HostNotFoundError(code);
        case WSA_NOT_ENOUGH_MEMORY:throw error::MemoryAllocationError(code);
        default:throw error::SocketException(code);
    }
}

void mapLastError(size_t code) {
    if (code != 0) mapError(WSAGetLastError());
}

}

#endif //SOCKETSYS_WINDOWS_WINSOCK_HPP_

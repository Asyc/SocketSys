#ifndef SOCKETSYS_WINDOWS_HPP
#define SOCKETSYS_WINDOWS_HPP

#include <string_view>

#include "socketsys/address.hpp"

namespace socketsys::tcp {

struct SocketConfig;

class WinSockProvider {
public:
    using Handle = uint64_t;
    using IpAddress = std::string_view;
    using Port = uint16_t;

    static constexpr Handle INVALID_HANDLE = ~(0);

    static Handle newSocket(AddressFamily family);
    static void destroy(Handle handle);

    static void connect(Handle handle, const IpAddress& remoteAddress, uint16_t remotePort);

    static void setSocketOptions(Handle handle, const SocketConfig& config);
    static void getSocketOptions(Handle handle, SocketConfig& out);
};

using PLATFORM_PROVIDER = WinSockProvider;

}   // namespace socketsys::tcp

#endif //SOCKETSYS_WINDOWS_HPP

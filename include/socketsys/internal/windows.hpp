#ifndef SOCKETSYS_WINDOWS_HPP
#define SOCKETSYS_WINDOWS_HPP

#include <string_view>

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
};

using PLATFORM_PROVIDER = WinSockProvider;

}   // namespace socketsys::tcp

#endif //SOCKETSYS_WINDOWS_HPP
